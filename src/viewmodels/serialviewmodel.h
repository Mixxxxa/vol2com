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

#include <QObject>
#include <memory>
#include "connectmethodbase.h"
//#include <QSerialPort>
#include "serialconnector.h"
#include <QtQml/qqmlregistration.h>

class QAbstractListModel;

namespace vol2com
{
  class SerialViewModel : public QObject
  {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(SerialConnector::BaudRate baudrate READ baudrate WRITE setBaudrate NOTIFY baudrateChanged)
    Q_PROPERTY(SerialConnector::DataBits dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(SerialConnector::Parity parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(SerialConnector::StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(SerialConnector::FlowControl flowControl READ flowControl WRITE setFlowControl NOTIFY flowControlChanged)
    Q_PROPERTY(bool autoReconnect READ autoReconnect WRITE setAutoReconnect NOTIFY autoReconnectChanged)

    Q_PROPERTY(bool arePortsAvailable READ arePortsAvailable NOTIFY arePortsAvailableChanged)
    Q_PROPERTY(bool availableForEdit READ availableForEdit NOTIFY availableForEditChanged)
    Q_PROPERTY(QAbstractListModel* portsModel READ portsModel CONSTANT)
    Q_PROPERTY(vol2com::ConnectMethodBase::State state READ state NOTIFY stateChanged)

  public:
    SerialViewModel(QObject* parent = nullptr);

    const QString &port() const noexcept { return m_port; }
    SerialConnector::BaudRate baudrate() const noexcept { return m_baudrate; }
    SerialConnector::DataBits dataBits() const noexcept { return m_dataBits; }
    SerialConnector::Parity parity() const noexcept { return m_parity; }
    SerialConnector::StopBits stopBits() const noexcept { return m_stopBits; }
    SerialConnector::FlowControl flowControl() const noexcept;
    bool autoReconnect() const noexcept { return m_autoReconnect; }

    bool arePortsAvailable() const { return m_arePortsAvailable; }
    bool availableForEdit() const { return m_availableForEdit; }
    QAbstractListModel* portsModel() const { return m_portsModel; }
    vol2com::ConnectMethodBase::State state() const { return m_state; }

  public slots:
    void setPort(const QString& port);
    void setBaudrate(SerialConnector::BaudRate baudrate);
    void setDataBits(SerialConnector::DataBits dataBits);
    void setParity(SerialConnector::Parity parity);
    void setStopBits(SerialConnector::StopBits stopBits);
    void setFlowControl(SerialConnector::FlowControl newFlowControl);
    void setAutoReconnect(bool autoReconnect);

    void setArePortsAvailable(bool arePortsAvailable);
    void requestConnect();
    void requestDisconnect();
    void updatePortsList();

  signals:
    void portChanged(const QString& port);
    void baudrateChanged(SerialConnector::BaudRate baudrate);
    void dataBitsChanged(SerialConnector::DataBits dataBits);
    void parityChanged(SerialConnector::Parity parity);
    void stopBitsChanged(SerialConnector::StopBits stopBits);
    void flowControlChanged(SerialConnector::FlowControl flowControl);
    void autoReconnectChanged(bool autoReconnect);

    void arePortsAvailableChanged(bool arePortsAvailable);
    void availableForEditChanged(bool availableForEdit);
    void stateChanged(vol2com::ConnectMethodBase::State state);

  private:
    std::shared_ptr<SerialConnector> m_serial;
    QString m_port;
    SerialConnector::BaudRate m_baudrate;
    SerialConnector::DataBits m_dataBits;
    SerialConnector::Parity m_parity;
    SerialConnector::StopBits m_stopBits;
    SerialConnector::FlowControl m_flowControl;
    bool m_autoReconnect;
    bool m_arePortsAvailable;
    bool m_availableForEdit;
    QAbstractListModel* m_portsModel;
    ConnectMethodBase::State m_state;

  private slots:
    void setState(const ConnectMethodBase::State& state);
    void setAvailableForEdit(bool availableForEdit);
    void onPortsModelRefreshed();
    void onStateChanged(const ConnectMethodBase::State& state);
  };
}
