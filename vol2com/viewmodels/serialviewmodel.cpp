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

#include "serialviewmodel.h"
#include "controller.h"
#include "serialconnector.h"
#include "datamodels/basicqmlmodel.h"

namespace vol2com
{
  SerialViewModel::SerialViewModel(QObject* parent)
    : QObject(parent)
    , m_serial(std::static_pointer_cast<SerialConnector>(Controller::getInstance().connector()))
    , m_port(m_serial->port())
    , m_baudrate(m_serial->baudrate())
    , m_dataBits(m_serial->dataBits())
    , m_parity(m_serial->parity())
    , m_stopBits(m_serial->stopBits())
    , m_autoReconnect(m_serial->autoReconnect())
    , m_arePortsAvailable(false)
    , m_availableForEdit(true)
    , m_portsModel(m_serial->portsModel())
    , m_state(m_serial->state())
  {
    onPortsModelRefreshed();
    setAvailableForEdit(m_serial->state() == ConnectMethodBase::State::Idle);

    // Port
    QObject::connect(m_serial.get(), &SerialConnector::portChanged,
                     this, &SerialViewModel::setPort);
    QObject::connect(this, &SerialViewModel::portChanged,
                     m_serial.get(), &SerialConnector::setPort);
    // Baudrate
    QObject::connect(m_serial.get(), &SerialConnector::baudrateChanged,
                     this, &SerialViewModel::setBaudrate);
    QObject::connect(this, &SerialViewModel::baudrateChanged,
                     m_serial.get(), &SerialConnector::setBaudrate);
    // DataBits
    QObject::connect(m_serial.get(), &SerialConnector::dataBitsChanged,
                     this, &SerialViewModel::setDataBits);
    QObject::connect(this, &SerialViewModel::dataBitsChanged,
                     m_serial.get(), &SerialConnector::setDataBits);
    // Parity
    QObject::connect(m_serial.get(), &SerialConnector::parityChanged,
                     this, &SerialViewModel::setParity);
    QObject::connect(this, &SerialViewModel::parityChanged,
                     m_serial.get(), &SerialConnector::setParity);
    // StopBits
    QObject::connect(m_serial.get(), &SerialConnector::stopBitsChanged,
                     this, &SerialViewModel::setStopBits);
    QObject::connect(this, &SerialViewModel::stopBitsChanged,
                     m_serial.get(), &SerialConnector::setStopBits);
    // AutoReconnect
    QObject::connect(m_serial.get(), &SerialConnector::autoReconnectChanged,
                     this, &SerialViewModel::setAutoReconnect);
    QObject::connect(this, &SerialViewModel::autoReconnectChanged,
                     m_serial.get(), &SerialConnector::setAutoReconnect);
    // Other
    QObject::connect(m_serial.get(), &SerialConnector::stateChanged,
                     this, &SerialViewModel::setState);
    QObject::connect(m_serial.get(), &SerialConnector::stateChanged,
                     this, &SerialViewModel::onStateChanged);
    QObject::connect(portsModel(), &QAbstractItemModel::modelReset,
                     this, &SerialViewModel::onPortsModelRefreshed);
  }

  void SerialViewModel::setPort(const QString &port)
  {
    if (m_port == port)
      return;

    m_port = port;
    emit portChanged(m_port);
  }

  void SerialViewModel::setBaudrate(SerialConnector::BaudRate baudrate)
  {
    if (m_baudrate == baudrate)
      return;

    m_baudrate = baudrate;
    emit baudrateChanged(m_baudrate);
  }

  void SerialViewModel::setDataBits(SerialConnector::DataBits dataBits)
  {
    if (m_dataBits == dataBits)
      return;

    m_dataBits = dataBits;
    emit dataBitsChanged(m_dataBits);
  }

  void SerialViewModel::setParity(SerialConnector::Parity parity)
  {
    if (m_parity == parity)
      return;

    m_parity = parity;
    emit parityChanged(m_parity);
  }

  void SerialViewModel::setStopBits(SerialConnector::StopBits stopBits)
  {
    if (m_stopBits == stopBits)
      return;

    m_stopBits = stopBits;
    emit stopBitsChanged(m_stopBits);
  }

  void SerialViewModel::setAutoReconnect(bool autoReconnect)
  {
    if (m_autoReconnect == autoReconnect)
      return;

    m_autoReconnect = autoReconnect;
    emit autoReconnectChanged(m_autoReconnect);
  }

  void SerialViewModel::requestConnect()
  {
    if(m_serial)
    {
      m_serial->requestConnect();
    }

    //QMetaObject::invokeMethod(m_serial.get(),
    //                          &SerialConnector::connectDevice, Qt::QueuedConnection);
  }

  void SerialViewModel::requestDisconnect()
  {
    if(m_serial)
    {
      m_serial->requestDisconnect();
    }
    //QMetaObject::invokeMethod(m_serial.get(),
    //                          &SerialConnector::disconnectDevice, Qt::QueuedConnection);
  }

  void SerialViewModel::updatePortsList()
  {
    if(m_serial)
      m_serial->updateSerialPorts();
  }

  void SerialViewModel::setArePortsAvailable(bool arePortsAvailable)
  {
    if (m_arePortsAvailable == arePortsAvailable)
      return;

    m_arePortsAvailable = arePortsAvailable;
    emit arePortsAvailableChanged(m_arePortsAvailable);
  }

  void SerialViewModel::setAvailableForEdit(bool availableForEdit)
  {
    if (m_availableForEdit == availableForEdit)
      return;

    m_availableForEdit = availableForEdit;
    emit availableForEditChanged(m_availableForEdit);
  }

  void SerialViewModel::setState(const ConnectMethodBase::State& state)
  {
    if (m_state == state)
      return;

    m_state = state;
    emit stateChanged(m_state);
  }

  void SerialViewModel::onPortsModelRefreshed()
  {
    setArePortsAvailable(m_portsModel->rowCount() > 0);
  }

  void SerialViewModel::onStateChanged(const ConnectMethodBase::State& state)
  {
    setAvailableForEdit(state == ConnectMethodBase::State::Idle);
  }

  SerialConnector::FlowControl SerialViewModel::flowControl() const noexcept
  {
    return m_flowControl;
  }

  void SerialViewModel::setFlowControl(SerialConnector::FlowControl newFlowControl)
  {
    if (m_flowControl == newFlowControl)
      return;
    m_flowControl = newFlowControl;
    emit flowControlChanged(m_flowControl);
  }

}
