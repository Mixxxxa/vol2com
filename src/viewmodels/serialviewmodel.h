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

#ifndef SERIALVIEWMODEL_H
#define SERIALVIEWMODEL_H

#include <QObject>
#include <memory>
#include "connectmethodbase.h"
#include <QSerialPort>

namespace vol2com
{
    class BasicQMLModel;
    class Serial;

    class SerialViewModel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
        Q_PROPERTY(QSerialPort::BaudRate baudrate READ baudrate WRITE setBaudrate NOTIFY baudrateChanged)
        Q_PROPERTY(QSerialPort::DataBits dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
        Q_PROPERTY(QSerialPort::Parity parity READ parity WRITE setParity NOTIFY parityChanged)
        Q_PROPERTY(QSerialPort::StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
        Q_PROPERTY(bool autoReconnect READ autoReconnect WRITE setAutoReconnect NOTIFY autoReconnectChanged)
        Q_PROPERTY(bool arePortsAvailable READ arePortsAvailable NOTIFY arePortsAvailableChanged)
        Q_PROPERTY(bool availableForEdit READ availableForEdit NOTIFY availableForEditChanged)
        Q_PROPERTY(vol2com::BasicQMLModel* portsModel READ portsModel CONSTANT)
        Q_PROPERTY(vol2com::ConnectMethodBase::State state READ state NOTIFY stateChanged)

    public:
        SerialViewModel(QObject* parent = nullptr);
        QString port() const { return m_port; }
        QSerialPort::BaudRate baudrate() const { return m_baudrate; }
        QSerialPort::DataBits dataBits() const { return m_dataBits; }
        QSerialPort::Parity parity() const { return m_parity; }
        QSerialPort::StopBits stopBits() const { return m_stopBits; }
        bool autoReconnect() const { return m_autoReconnect; }
        bool arePortsAvailable() const { return m_arePortsAvailable; }
        bool availableForEdit() const { return m_availableForEdit; }
        vol2com::BasicQMLModel* portsModel() const { return m_portsModel; }
        vol2com::ConnectMethodBase::State state() const { return m_state; }

    public slots:
        void setPort(const QString& port);
        void setBaudrate(const QSerialPort::BaudRate& baudrate);
        void setDataBits(const QSerialPort::DataBits& dataBits);
        void setParity(const QSerialPort::Parity& parity);
        void setStopBits(const QSerialPort::StopBits& stopBits);
        void setAutoReconnect(bool autoReconnect);
        void setArePortsAvailable(bool arePortsAvailable);
        void connectDevice();
        void disconnectDevice();
        void updatePortsList();

    signals:
        void portChanged(const QString& port);
        void baudrateChanged(const QSerialPort::BaudRate& baudrate);
        void dataBitsChanged(const QSerialPort::DataBits& dataBits);
        void parityChanged(const QSerialPort::Parity& parity);
        void stopBitsChanged(const QSerialPort::StopBits& stopBits);
        void autoReconnectChanged(bool autoReconnect);
        void arePortsAvailableChanged(bool arePortsAvailable);
        void availableForEditChanged(bool availableForEdit);
        void stateChanged(vol2com::ConnectMethodBase::State state);

    private:
        std::shared_ptr<Serial> m_serial;
        QString m_port;
        QSerialPort::BaudRate m_baudrate;
        QSerialPort::DataBits m_dataBits;
        QSerialPort::Parity m_parity;
        QSerialPort::StopBits m_stopBits;
        bool m_autoReconnect;
        bool m_arePortsAvailable;
        bool m_availableForEdit;
        BasicQMLModel* m_portsModel;
        ConnectMethodBase::State m_state;

    private slots:
        void setState(const ConnectMethodBase::State& state);
        void setAvailableForEdit(bool availableForEdit);
        void onPortsModelRefreshed();
        void onStateChanged(const ConnectMethodBase::State& state);
    };
}

#endif // SERIALVIEWMODEL_H
