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

#include "serial.h"
#include "serialport.h"
#include "datamodels/basicqmlmodel.h"
#include "settings.h"
#include <QSerialPortInfo>
#include <QThread>
#include <QRegExp>

using namespace vol2com;

namespace
{
    static constexpr auto Name = "Serial";

    bool isPortNameCorrect(const QString& name)
    {
        QRegExp exp("^COM\\d+$");
        return exp.exactMatch(name);
    }
}

Serial::Serial(QObject *parent) :
    ConnectMethodBase(parent),
    m_port(Serial::UnknownPort),
    m_baudrate(QSerialPort::Baud57600),
    m_dataBits(QSerialPort::Data8),
    m_parity(QSerialPort::NoParity),
    m_stopBits(QSerialPort::OneStop),
    m_autoReconnect(true),
    m_portsModel(new BasicQMLModel(this)),
    m_serialThread(nullptr),
    m_serial(nullptr)
{
    updateSerialPorts();
    load();
    setState(State::Idle);
}

Serial::~Serial()
{
    save();
    onConnectionClosed();
}

QString Serial::qmlDelegate() const
{
    return ConnectMethodBase::qmlDelegatesPath() + QStringLiteral("SerialDelegate.qml");
}

#include <QDebug>

bool Serial::isPortAvailable(const QString &port) const
{
    return (m_portsModel->findIndex(port) != -1);
}

void Serial::setPort(const QString &port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(m_port);
}

void Serial::setBaudrate(const QSerialPort::BaudRate &baudrate)
{
    if (m_baudrate == baudrate)
        return;

    m_baudrate = baudrate;
    emit baudrateChanged(m_baudrate);
}

void Serial::setDataBits(const QSerialPort::DataBits &dataBits)
{
    if (m_dataBits == dataBits)
        return;

    m_dataBits = dataBits;
    emit dataBitsChanged(m_dataBits);
}

void Serial::setParity(const QSerialPort::Parity &parity)
{
    if (m_parity == parity)
        return;

    m_parity = parity;
    emit parityChanged(m_parity);
}

void Serial::setStopBits(const QSerialPort::StopBits &stopBits)
{
    if (m_stopBits == stopBits)
        return;

    m_stopBits = stopBits;
    emit stopBitsChanged(m_stopBits);
}

void Serial::setAutoReconnect(bool autoReconnect)
{
    if (m_autoReconnect == autoReconnect)
        return;

    m_autoReconnect = autoReconnect;
    emit autoReconnectChanged(m_autoReconnect);
}

void Serial::updateSerialPorts()
{
    m_portsModel->clear();
    for(const auto& port : QSerialPortInfo::availablePorts())
    {
        m_portsModel->add(QStringLiteral("%1 - %2").arg(port.portName()).arg(port.description()),
                          port.portName());
    }
    if(m_portsModel->rowCount() == 0)
        m_portsModel->add(QStringLiteral("Unavailable"), Serial::UnknownPort);
    emit portsModelRefreshed();
}

void Serial::save()
{
    auto& settings = Settings::getInstance();
    settings.set(QString(::Name), SettingsKeys::Port, m_port);
    settings.set(QString(::Name), SettingsKeys::Baudrate, m_baudrate);
    settings.set(QString(::Name), SettingsKeys::Databits, m_dataBits);
    settings.set(QString(::Name), SettingsKeys::Parity, m_parity);
    settings.set(QString(::Name), SettingsKeys::StopBits, m_stopBits);
    settings.set(QString(::Name), SettingsKeys::AutoReconnect, m_autoReconnect);
}

void Serial::load()
{
    auto& settings = Settings::getInstance();

    const auto port = settings.get(QString(::Name), SettingsKeys::Port, Serial::UnknownPort).toString();
    if(isPortNameCorrect(port) && isPortAvailable(port))
        m_port = port;
    else if (m_portsModel->rowCount() > 0)
        m_port = m_portsModel->data(m_portsModel->index(0),  BasicQMLModel::ValueRole).toString();
    else
        m_port = Serial::UnknownPort;

    m_baudrate = settings.getEnum(QString(::Name), SettingsKeys::Baudrate, QSerialPort::Baud57600);
    m_dataBits = settings.getEnum(QString(::Name), SettingsKeys::Databits, QSerialPort::Data8);
    m_parity = settings.getEnum(QString(::Name), SettingsKeys::Parity, QSerialPort::NoParity);
    m_stopBits = settings.getEnum(QString(::Name), SettingsKeys::StopBits, QSerialPort::OneStop);
    m_autoReconnect = settings.get(QString(::Name), SettingsKeys::AutoReconnect, true).toBool();
}

void Serial::write(const QByteArray& data)
{
    if(m_serial && state() == State::Connected)
    {
        emit send(data);
    }
}

void Serial::connectDevice()
{
    if(m_serial)
        return;

    if(m_port == Serial::UnknownPort)
    {
        emit connectionFailed(tr("No ports available for connection"));
        return;
    }

    setState(ConnectMethodBase::State::Connecting);
    m_serialThread = new QThread(this);
    m_serial = new SerialPort(m_port);
    m_serial->moveToThread(m_serialThread);

    m_serial->setBaudRate(static_cast<SerialPort::BaudRate>(m_baudrate));
    m_serial->setFlowControl(SerialPort::NoFlowControl);
    m_serial->setParity(static_cast<SerialPort::Parity>(m_parity));
    m_serial->setDataBits(static_cast<SerialPort::DataBits>(m_dataBits));
    m_serial->setStopBits(static_cast<SerialPort::StopBits>(m_stopBits));

    QObject::connect(m_serialThread, &QThread::started,
                     m_serial, &SerialPort::openPort);
    QObject::connect(m_serialThread, &QThread::finished,
                     m_serial, &SerialPort::deleteLater);

    QObject::connect(m_serial, &SerialPort::connectionSuccess,
                     this, &Serial::onConnectionSuccess);
    QObject::connect(m_serial, &SerialPort::connectionFailed,
                     this, &Serial::onConnectionFailed);
    QObject::connect(m_serial, &SerialPort::connectionClosed,
                     this, &Serial::onConnectionClosed);

    m_serialThread->start();
}

void Serial::disconnectDevice()
{
    QMetaObject::invokeMethod(m_serial, &SerialPort::closePort, Qt::QueuedConnection);
}

void Serial::reconnect()
{
    connectDevice();
}

void Serial::onConnectionSuccess()
{
    if(state() != State::Reconnecting)
    {
        QObject::connect(this, &Serial::send,
                         m_serial, &SerialPort::formatAndSend,
                         Qt::QueuedConnection);
        QObject::connect(m_serial, &SerialPort::errorOccurred,
                         this, &Serial::onErrorOccured);
    }
    setState(State::Connected);
}

void Serial::onConnectionFailed()
{
    const auto errorText = QStringLiteral("%1 - %2").arg(m_port, m_serial->errorString());
    emit connectionFailed(errorText);
    onConnectionClosed();
}

void Serial::onConnectionClosed()
{
    if(m_serial)
    {
        m_serialThread->exit();
        m_serialThread->wait();

        m_serial = nullptr;
        delete m_serialThread;
        m_serialThread = nullptr;
    }

    setState(State::Idle);
}

void Serial::onErrorOccured(const QSerialPort::SerialPortError& error)
{
    if(error == QSerialPort::SerialPortError::NoError)
        return;
    if(state() == State::Reconnecting)
        return;

    if(autoReconnect())
    {
        setState(State::Reconnecting);
        QMetaObject::invokeMethod(m_serial, &SerialPort::reconnect, Qt::QueuedConnection);
    }
    else
    {
        disconnectDevice();
    }
}
