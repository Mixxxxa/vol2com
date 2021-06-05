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

#ifndef SERIAL_H
#define SERIAL_H

#include "connectmethodbase.h"
#include <QSerialPort>

class QThread;

namespace vol2com
{
    class BasicQMLModel;
    class SerialPort;

    class Serial : public ConnectMethodBase
    {
        Q_OBJECT

    public:
        static constexpr auto UnknownPort = "COM0";

        enum class SettingsKeys
        {
            Port,
            Baudrate,
            Databits,
            Parity,
            StopBits,
            AutoReconnect
        };
        Q_ENUM(SettingsKeys)

        explicit Serial(QObject *parent = nullptr);
        ~Serial();

        QString port() const { return m_port; }
        QSerialPort::BaudRate baudrate() const { return m_baudrate; }
        QSerialPort::DataBits dataBits() const { return m_dataBits; }
        QSerialPort::Parity parity() const { return m_parity; }
        QSerialPort::StopBits stopBits() const { return m_stopBits; }
        bool autoReconnect() const { return m_autoReconnect; }
        BasicQMLModel* portsModel() const { return m_portsModel; }
        virtual QString qmlDelegate() const override;
        bool isPortAvailable(const QString& port) const;

    public slots:
        void setPort(const QString& port);
        void setBaudrate(const QSerialPort::BaudRate& baudrate);
        void setDataBits(const QSerialPort::DataBits& dataBits);
        void setParity(const QSerialPort::Parity& parity);
        void setStopBits(const QSerialPort::StopBits& stopBits);
        void setAutoReconnect(bool autoReconnect);
        void updateSerialPorts();

        virtual void save() override;
        virtual void load() override;
        virtual void write(const QByteArray& data) override;
        virtual void connectDevice() override;
        virtual void disconnectDevice() override;
        virtual void reconnect() override;

    signals:
        void portChanged(const QString& port);
        void baudrateChanged(const QSerialPort::BaudRate& baudrate);
        void dataBitsChanged(const QSerialPort::DataBits& dataBits);
        void parityChanged(const QSerialPort::Parity& parity);
        void stopBitsChanged(const QSerialPort::StopBits& stopBits);
        void autoReconnectChanged(bool autoReconnect);
        void send(const QByteArray& data);
        void portsModelRefreshed();

    private:
        QString m_port;
        QSerialPort::BaudRate m_baudrate;
        QSerialPort::DataBits m_dataBits;
        QSerialPort::Parity m_parity;
        QSerialPort::StopBits m_stopBits;
        bool m_autoReconnect;
        BasicQMLModel* m_portsModel;

        QThread* m_serialThread;
        SerialPort* m_serial;

    private slots:
        void onConnectionSuccess();
        void onConnectionFailed();
        void onConnectionClosed();
        void onErrorOccured(const QSerialPort::SerialPortError& error);
    };
}

Q_DECLARE_METATYPE( vol2com::Serial* )

#endif // SERIAL_H
