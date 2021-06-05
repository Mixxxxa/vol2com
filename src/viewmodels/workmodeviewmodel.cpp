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

#include "workmodeviewmodel.h"
#include "controller.h"
#include "basslibwrapper.h"
#include "workmodesfactory.h"

using namespace vol2com;

WorkModeViewModel::WorkModeViewModel(QObject* parent) :
    QObject(parent),
    m_bassLib(Controller::getInstance().bassLib()),
    m_modesModel(Controller::getInstance().factory()->model()),
    m_currentMode(Controller::getInstance().mode()),
    m_currentDevice(-1),
    m_deviceError(false)
{
    setCurrentDevice(m_bassLib->device());
    onBassStateChanged(m_bassLib->state());

    QObject::connect(m_bassLib.get(), &BassLibWrapper::deviceChanged,
                     this, &WorkModeViewModel::setCurrentDevice);
    QObject::connect(m_bassLib.get(), &BassLibWrapper::stateChanged,
                     this, &WorkModeViewModel::onBassStateChanged);
    QObject::connect(this, &WorkModeViewModel::currentDeviceChanged,
                     m_bassLib.get(), &BassLibWrapper::setDevice);

    auto& controller = Controller::getInstance();
    QObject::connect(&controller, &Controller::modeChanged,
                     this, &WorkModeViewModel::setMode);
    QObject::connect(this, &WorkModeViewModel::selectMode,
                     &controller, &Controller::setMode);
    QObject::connect(this, &WorkModeViewModel::clearMode,
                     &controller, &Controller::clearMode);
}

WorkModesModel* WorkModeViewModel::modes() const
{
    return m_modesModel.get();
}

BasicQMLModel* WorkModeViewModel::devices() const
{
    return m_bassLib->devicesModel().get();
}

int WorkModeViewModel::currentDevice() const
{
    return m_currentDevice;
}

bool WorkModeViewModel::deviceError() const
{
    return m_deviceError;
}

QString WorkModeViewModel::deviceErrorText() const
{
    return m_deviceErrorText;
}

WorkModeBase* WorkModeViewModel::currentMode() const
{
    return m_currentMode.get();
}

void WorkModeViewModel::setCurrentDevice(int currentDevice)
{
    if (m_currentDevice == currentDevice)
        return;

    m_currentDevice = currentDevice;
    emit currentDeviceChanged(m_currentDevice);
}

void WorkModeViewModel::setMode(std::shared_ptr<WorkModeBase> mode)
{
    if (m_currentMode == mode)
        return;

    m_currentMode = mode;
    emit currentModeChanged(m_currentMode.get());
}

void WorkModeViewModel::onBassStateChanged(const BassLibWrapper::State& state)
{
    const auto isError = (state == BassLibWrapper::State::Error);
    if(isError)
    {
        m_deviceErrorText = "ssss";// m_bassLib->errorText();
        emit deviceErrorTextChanged(m_deviceErrorText);
    }

    setDeviceError(isError);
}

void WorkModeViewModel::setDeviceError(bool deviceError)
{
    if (m_deviceError == deviceError)
        return;

    m_deviceError = deviceError;
    emit deviceErrorChanged(m_deviceError);
}
