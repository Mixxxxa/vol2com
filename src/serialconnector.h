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

#pragma once

#include "connectmethodbase.h"
#include <QSerialPort>
#include <QtQml/qqmlregistration.h>

class QThread;

namespace vol2com
{
  class SerialWorker;
  class BasicQMLModel;

  class SerialConnector : public ConnectMethodBase
  {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

  public:
    static constexpr auto UnknownPort = "COM0";

    enum class BaudRate
    {
        Baud1200   = QSerialPort::Baud1200
      , Baud2400   = QSerialPort::Baud2400
      , Baud4800   = QSerialPort::Baud4800
      , Baud9600   = QSerialPort::Baud9600
      , Baud19200  = QSerialPort::Baud19200
      , Baud38400  = QSerialPort::Baud38400
      , Baud57600  = QSerialPort::Baud57600
      , Baud115200 = QSerialPort::Baud115200
    };
    Q_ENUM(BaudRate)

    enum class DataBits
    {
        Data5 = QSerialPort::Data5
      , Data6 = QSerialPort::Data6
      , Data7 = QSerialPort::Data7
      , Data8 = QSerialPort::Data8
    };
    Q_ENUM(DataBits)

    enum class Parity
    {
        NoParity    = QSerialPort::NoParity
      , EvenParity  = QSerialPort::EvenParity
      , OddParity   = QSerialPort::OddParity
      , SpaceParity = QSerialPort::SpaceParity
      , MarkParity  = QSerialPort::MarkParity
    };
    Q_ENUM(Parity)

    enum class StopBits
    {
        OneStop        = QSerialPort::TwoStop
      , OneAndHalfStop = QSerialPort::OneAndHalfStop
      , TwoStop        = QSerialPort::OneStop
    };
    Q_ENUM(StopBits)

    enum class FlowControl
    {
        NoFlowControl   = QSerialPort::NoFlowControl
      , HardwareControl = QSerialPort::HardwareControl
      , SoftwareControl = QSerialPort::SoftwareControl
    };
    Q_ENUM(FlowControl)

    enum class SettingsKeys
    {
        Port
      , Baudrate
      , Databits
      , Parity
      , StopBits
      , AutoReconnect
    };
    Q_ENUM(SettingsKeys)

    explicit SerialConnector(QObject *parent = nullptr);
    ~SerialConnector();

    const QString &port() const noexcept;
    SerialConnector::BaudRate baudrate() const noexcept;
    SerialConnector::DataBits dataBits() const noexcept;
    SerialConnector::Parity parity() const noexcept;
    SerialConnector::StopBits stopBits() const noexcept;
    SerialConnector::FlowControl flowControl() const noexcept;
    bool autoReconnect() const noexcept;

    BasicQMLModel *portsModel() const;
    virtual QString qmlDelegate() const override;
    bool isPortAvailable(const QString& port) const;

  public slots:
    void setPort(const QString& port);
    void setBaudrate(SerialConnector::BaudRate baudrate);
    void setDataBits(SerialConnector::DataBits dataBits);
    void setParity(SerialConnector::Parity parity);
    void setStopBits(SerialConnector::StopBits stopBits);
    void setFlowControl(SerialConnector::FlowControl newFlowControl);
    void setAutoReconnect(bool autoReconnect);

    void updateSerialPorts();

    virtual void save() override;
    virtual void load() override;
    virtual void write(const QByteArray& data) override;
    virtual void requestConnect() override;
    virtual void requestDisconnect() override;
    virtual void requestReconnect() override;

  signals:
    void portChanged(const QString& port);
    void baudrateChanged(SerialConnector::BaudRate baudrate);
    void dataBitsChanged(SerialConnector::DataBits dataBits);
    void parityChanged(SerialConnector::Parity parity);
    void stopBitsChanged(SerialConnector::StopBits stopBits);
    void flowControlChanged(SerialConnector::FlowControl newFlowControl);
    void autoReconnectChanged(bool autoReconnect);

  private:
    Q_SIGNAL void send(const QByteArray& data);

  private:
    BasicQMLModel *m_portsModel   = nullptr;
    QThread       *m_workerThread = nullptr;
    SerialWorker  *m_worker       = nullptr;

    QString     m_port;
    BaudRate    m_baudrate      = BaudRate::Baud57600;
    DataBits    m_dataBits      = DataBits::Data8;
    Parity      m_parity        = Parity::NoParity;
    StopBits    m_stopBits      = StopBits::OneStop;
    FlowControl m_flowControl   = FlowControl::NoFlowControl;
    bool        m_autoReconnect = true;

  private slots:
    void stop();

    void onConnected();
    void onFailedToConnect();
    //void onDisconnected();


    //void onConnectionSuccess();
    //void onConnectionFailed();
    //void onConnectionClosed();
    void onErrorOccured(const QSerialPort::SerialPortError& error);
  };
}

//Q_DECLARE_METATYPE( vol2com::Serial* )
