// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/****************************************************************************
**
** Copyright (C) 2020-2021 Mixxxxa <gelvikhmikhail@gmail.com>.
**
** This file is part of vol2com.
**
** vol2com is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** any later version.
**
** vol2com is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with vol2com. If not, see <https://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "basslibwrapper.h"
#include "./libs/bass/c/bass.h"
#include "./libs/bass_wasapi/c/basswasapi.h"
#include "datamodels/basicqmlmodel.h"
#include "controller.h"
#include <stdexcept>
#include <QMetaObject>
#include <QDebug>

using namespace vol2com;

namespace
{
    static constexpr uint16_t FFTSamples = 1024;

    //Cache for calculations. Calcualted with this expression:
    //i = round(pow(2, static_cast<double>(x * 10) / (BassLibWrapper::AvailableBands - 1)));
    //TODO Write constexpr function
    static constexpr std::array<size_t, BassLibWrapper::AvailableBands> B1Cache =
    {
        1, 1, 2, 3, 4, 6, 9, 13, 19, 27, 38, 55, 80, 115, 165, 238, 343, 494, 711, 1023
    };

    //Helper function for BassWASAPI. See BASS_WASAPI_Init docs
    DWORD CALLBACK WasapiProc(void *buffer, DWORD length, void *user)
    {
        Q_UNUSED(buffer)
        Q_UNUSED(length)
        Q_UNUSED(user)
        return static_cast<DWORD>(1);
    }

    void CALLBACK WasapiNotifyProc(DWORD notify, DWORD device, void *user)
    {
        Q_UNUSED(user)
        QMetaObject::invokeMethod(Controller::getInstance().bassLib().get(),
                                  "onBassEvent",
                                  Q_ARG(DWORD, notify),
                                  Q_ARG(DWORD, device));
    }

    int getLoopbackDevice(int deviceID)
    {
        BASS_WASAPI_DEVICEINFO device;
        if(BASS_WASAPI_GetDeviceInfo(deviceID, &device))
        {
            // Input devices not use loopback
            //if(device.flags&BASS_DEVICE_INPUT)
            //    return deviceID;

            auto deviceGUID = device.id;
            for (DWORD i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); ++i)
            {
                if(device.flags&BASS_DEVICE_LOOPBACK && deviceGUID == device.id)
                    return i;
            }
        }
        return BassLibWrapper::UnknownDevice;
    }
}

BassLibWrapper::BassLibWrapper() :
    m_state(State::Idle),
    m_device(BassLibWrapper::UnknownDevice),
    m_fft(nullptr),
    m_devicesModel(std::make_shared<BasicQMLModel>())
{
    qRegisterMetaType<DWORD>();
    load();
    updateDevicesModel();
    BASS_WASAPI_SetNotify(::WasapiNotifyProc, nullptr);
    QObject::connect(this, &BassLibWrapper::deviceChanged,
                     this, &BassLibWrapper::onDeviceChanged);
}

BassLibWrapper::~BassLibWrapper()
{
    this->stop();
}

std::vector<std::pair<QString, int> > BassLibWrapper::availableDevices()
{
    BASS_WASAPI_DEVICEINFO device;
    std::vector<std::pair<QString, int> > result;

    result.emplace_back(tr("Default output device"), BassLibWrapper::DefaultOutputDevice);
    result.emplace_back(tr("Default input device"), BassLibWrapper::DefaultInputDevice);

    for (DWORD i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); ++i)
    {
        if(device.flags&BASS_DEVICE_ENABLED && !(device.flags&BASS_DEVICE_LOOPBACK))
        {
            result.emplace_back(QString::fromLocal8Bit(device.name), static_cast<int>(i));
        }
    }

    return result;
}

QString BassLibWrapper::getDeviceName(int deviceID)
{
    BASS_WASAPI_DEVICEINFO device;
    const bool check = BASS_WASAPI_GetDeviceInfo(static_cast<DWORD>(deviceID), &device);
    if(check)
        return QString::fromLocal8Bit(device.name);
    else
        return QString("Unknown");
}

int BassLibWrapper::defaultInputDeviceID()
{
    BASS_WASAPI_DEVICEINFO device;
    for (DWORD i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); ++i)
        if(device.flags&BASS_DEVICE_ENABLED && device.flags&BASS_DEVICE_DEFAULT && device.flags&BASS_DEVICE_INPUT)
            return static_cast<int>(i);
    return BassLibWrapper::UnknownDevice;
}

int BassLibWrapper::defaultOutputDeviceID()
{
    BASS_WASAPI_DEVICEINFO device;
    for (DWORD i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); ++i)
        if(device.flags&BASS_DEVICE_ENABLED && device.flags&BASS_DEVICE_DEFAULT && !(device.flags&BASS_DEVICE_INPUT))
            return static_cast<int>(i);
    return BassLibWrapper::UnknownDevice;
}

bool BassLibWrapper::start()
{
    try
    {
        init();
        return true;
    }
    catch (std::runtime_error& ex)
    {
        stop();
        setState(State::Error);
        emit showNotification("Failed to capture",
                              ex.what());
        return false;
    };
}

void BassLibWrapper::stop()
{
    if(m_state != State::Idle)
    {
        BASS_WASAPI_Free();
        BASS_Free();

        if(m_fft)
        {
            delete[] m_fft;
            m_fft = nullptr;
        }
        m_state = State::Idle;
    }
}

void BassLibWrapper::init()
{
    const int targetDevice = getRealDevice();
    const int loopbackDevice = ::getLoopbackDevice(targetDevice);

    if(loopbackDevice == BassLibWrapper::UnknownDevice)
    {
        const QString deviceName = getDeviceName(targetDevice);
        QString errorString;

        if(m_device == DefaultInputDevice)
            errorString = tr("Unable to capture from default input device %1 (%2)").arg(deviceName).arg(targetDevice);
        else if(m_device == DefaultOutputDevice)
            errorString = tr("Unable to capture from default output device %1 (%2)").arg(deviceName).arg(targetDevice);
        else
            errorString = tr("Unable to capture from device %1 (%2)").arg(deviceName).arg(targetDevice);

        throw std::runtime_error(errorString.toStdString());
    }

    if (HIWORD(BASS_GetVersion()) != BASSVERSION)
        throw std::runtime_error("Incorrect BASS lib version loaded");

    if (!BASS_Init(0, 44100, 0, nullptr, nullptr))
        throw std::runtime_error("BassLib init failed");

    if (!BASS_WASAPI_Init(loopbackDevice, 0, 0, BASS_WASAPI_BUFFER, 1, 0.1f, &::WasapiProc, NULL))
        throw std::runtime_error("BassWASAPILib init failed");

    if(!BASS_WASAPI_Start())
        throw std::runtime_error("BassWASAPILib start failed");

    m_fft = new float[::FFTSamples];
    setState(State::Active);
    qDebug() << "Bass initiated with device #" << targetDevice << "(" << loopbackDevice << ")" << getDeviceName(targetDevice);
}

void BassLibWrapper::updateDevicesModel()
{
    m_devicesModel->clear();
    for(const auto& item : BassLibWrapper::availableDevices())
        m_devicesModel->add(item.first, item.second);
}

void BassLibWrapper::setDevice(int device)
{
    if (m_device == device)
        return;

    m_device = device;
    emit deviceChanged(m_device);
}

void BassLibWrapper::load()
{
    m_device = BassLibWrapper::DefaultOutputDevice;
}

void BassLibWrapper::save()
{

}

BassLibWrapper::BassLibData BassLibWrapper::allBands()
{
    if(m_state != State::Active)
        return BassLibData{ 0 };

    BassLibData result;
    BASS_WASAPI_GetData(m_fft, BASS_DATA_FFT2048);

    int y = 0;
    size_t b0 = 0;
    float peak;

    for (size_t x = 0; x < AvailableBands; ++x)
    {
        peak = 0;
        size_t b1 = ::B1Cache[x];
        if (b1 <= b0)
            b1 = b0+1;
        for (; b0 < b1; ++b0)
            if (peak < m_fft[1+b0])
                peak = m_fft[1+b0];

        y = std::round(std::sqrt(peak) * 3 * MaxValue);

        if(y > MaxValue)
            y = MaxValue;
        else if (y < 0)
            y = 0;

        result[x] = static_cast<uint8_t>(y);
    }
    return result;
}

uint8_t BassLibWrapper::band(int band)
{
    if((m_state != State::Active) || (band < 0) || (band >= AvailableBands))
        return static_cast<uint8_t>(0);

    BASS_WASAPI_GetData(m_fft, BASS_DATA_FFT2048);

    size_t b0 = 0;
    float peak;
    int result = 0;

    for (int x = 0; x < AvailableBands; ++x)
    {
        peak = 0;
        size_t b1 = ::B1Cache[x];
        if (b1 <= b0)
            b1 = b0+1;
        for (; b0 < b1; ++b0)
            if (peak < m_fft[1+b0])
                peak = m_fft[1+b0];

        if(x == band)
        {
            result = std::round(std::sqrt(peak) * 3 * MaxValue);
            if(result > MaxValue)
                result = MaxValue;
            else if (result < 0)
                result = 0;
            break;
        }
    }
    return result;
}

void BassLibWrapper::onBassEvent(const DWORD& event, const DWORD& device)
{
    switch(event)
    {
    case BASS_WASAPI_NOTIFY_DEFINPUT:
    case BASS_WASAPI_NOTIFY_DEFOUTPUT:
        if(m_device == DefaultInputDevice || m_device == DefaultOutputDevice)
        {
            onDeviceChanged();
        }
        break;
    case BASS_WASAPI_NOTIFY_FAIL:
        onDeviceFailed(device);
    }
}

void BassLibWrapper::setState(const BassLibWrapper::State& state)
{
    if(m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}

QString BassLibWrapper::getErrorText(int errorCode) const
{
    QString temp = QString("%1 - ").arg(errorCode);

    switch(errorCode){
    case BASS_OK:
        temp.append(tr("No errors detected"));
        break;
    case BASS_ERROR_DEVICE:
        temp.append(tr("Failed to use device"));
        break;
    case BASS_ERROR_ALREADY:
        temp.append(tr("The device has already been initialized"));
        break;
    case BASS_ERROR_DRIVER:
        temp.append(tr("There is no available device driver"));
        break;
    case BASS_ERROR_BUSY:
        temp.append(tr("Something else has exclusive use of the device"));
        break;
    case BASS_ERROR_FORMAT:
        temp.append(tr("The specified format is not supported by the device"));
        break;
    case BASS_ERROR_MEM:
        temp.append(tr("Memory error"));
        break;
    case BASS_ERROR_INIT:
        temp.append(tr("Current device is not initialized"));
        break;
    default:
        temp.append(tr("Unknown error"));
        break;
    }

    return temp;
}

int BassLibWrapper::getRealDevice() const
{
    if(m_device == BassLibWrapper::DefaultOutputDevice)
        return BassLibWrapper::defaultOutputDeviceID();
    else if(m_device == BassLibWrapper::DefaultInputDevice)
        return BassLibWrapper::defaultInputDeviceID();
    else
        return m_device;
}

void BassLibWrapper::onDeviceChanged()
{
    if(state() != State::Idle)
    {
        try
        {
            stop();
            init();
            emit showNotification(tr("Capture device changed"),
                                  getDeviceName(getRealDevice()));
        }
        catch (std::runtime_error& ex)
        {
            stop();
            setState(State::Error);
            emit showNotification("Failed to change device",
                                  ex.what());
        }
    }
}

void BassLibWrapper::onDeviceFailed(int device)
{
    //If failed device is currently used
    if(device == ::getLoopbackDevice(getRealDevice()))
    {
        qDebug() << "Device failed";
        stop();
        setState(State::Error);
        emit showNotification(tr("Device failed"),
                              getDeviceName(getRealDevice()));
    }
}
