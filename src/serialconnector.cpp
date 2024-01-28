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

#include "serialconnector.h"
#include "serialworker.h"
#include <QSerialPortInfo>
#include "datamodels/basicqmlmodel.h"
#include "settings.h"
#pragma warning ( push )
#pragma warning( disable : 5204)
#include <QThread> // Disable C5204 from ppwin.h that is used by <future>
#pragma warning ( pop )
#include <QRegularExpression>

namespace
{
  static constexpr auto Name = "Serial";

  bool isPortNameCorrect(const QString& name)
  {
    QRegularExpression re(R"delim(^COM\d+$)delim");
    return re.match(name).hasMatch();
  }
}

namespace vol2com
{
  SerialConnector::SerialConnector(QObject *parent)
    : ConnectMethodBase(parent)
    , m_portsModel(new BasicQMLModel(this))
    , m_workerThread(nullptr)
    , m_worker(nullptr)
  {
    updateSerialPorts();
    setState(State::Idle);
  }

  SerialConnector::~SerialConnector()
  {
    stop();
  }

  const QString &SerialConnector::port() const noexcept
  {
    return m_port;
  }

  SerialConnector::BaudRate SerialConnector::baudrate() const noexcept
  {
    return m_baudrate;
  }

  SerialConnector::DataBits SerialConnector::dataBits() const noexcept
  {
    return m_dataBits;
  }

  SerialConnector::Parity SerialConnector::parity() const noexcept
  {
    return m_parity;
  }

  SerialConnector::StopBits SerialConnector::stopBits() const noexcept
  {
    return m_stopBits;
  }

  SerialConnector::FlowControl SerialConnector::flowControl() const noexcept
  {
    return m_flowControl;
  }

  bool SerialConnector::autoReconnect() const noexcept
  {
    return m_autoReconnect;
  }

  void SerialConnector::setPort(const QString &port)
  {
    if (m_port == port)
    {
      return;
    }

    m_port = port;
    emit portChanged(m_port);
  }

  void SerialConnector::setBaudrate(BaudRate baudrate)
  {
    if (m_baudrate == baudrate)
    {
      return;
    }

    m_baudrate = baudrate;
    emit baudrateChanged(m_baudrate);
  }

  void SerialConnector::setDataBits(DataBits dataBits)
  {
    if (m_dataBits == dataBits)
    {
      return;
    }

    m_dataBits = dataBits;
    emit dataBitsChanged(m_dataBits);
  }

  void SerialConnector::setParity(Parity parity)
  {
    if (m_parity == parity)
    {
      return;
    }

    m_parity = parity;
    emit parityChanged(m_parity);
  }

  void SerialConnector::setStopBits(StopBits stopBits)
  {
    if (m_stopBits == stopBits)
    {
      return;
    }

    m_stopBits = stopBits;
    emit stopBitsChanged(m_stopBits);
  }

  void SerialConnector::setAutoReconnect(bool autoReconnect)
  {
    if (m_autoReconnect == autoReconnect)
    {
      return;
    }

    m_autoReconnect = autoReconnect;
    emit autoReconnectChanged(m_autoReconnect);
  }

  void SerialConnector::setFlowControl(SerialConnector::FlowControl newFlowControl)
  {
    if (m_flowControl == newFlowControl)
    {
      return;
    }

    m_flowControl = newFlowControl;
    emit flowControlChanged(m_flowControl);
  }

  QString SerialConnector::qmlDelegate() const
  {
    return ConnectMethodBase::qmlDelegatesPath() + QStringLiteral("SerialDelegate.qml");
  }

  bool SerialConnector::isPortAvailable(const QString &port) const
  {
    return (m_portsModel->findIndex(port) != -1);
  }

  void SerialConnector::updateSerialPorts()
  {
    using namespace Qt::StringLiterals;

    BasicQMLModel::container_type newPorts;
    for(const auto &port : QSerialPortInfo::availablePorts())
    {
      newPorts.emplace_back(
        BasicQMLModel::container_type::value_type{
          .text = u"%1 - %2"_s.arg(port.portName(), port.description()),
          .value = port.portName()
      });
    }
    m_portsModel->SetData(newPorts);
  }

  void SerialConnector::save()
  {
    //auto& settings = Settings::getInstance();
    //settings.set(QString(::Name), SettingsKeys::Port, m_port);
    //settings.set(QString(::Name), SettingsKeys::Baudrate, m_baudrate);
    //settings.set(QString(::Name), SettingsKeys::Databits, m_dataBits);
    //settings.set(QString(::Name), SettingsKeys::Parity, m_parity);
    //settings.set(QString(::Name), SettingsKeys::StopBits, m_stopBits);
    //settings.set(QString(::Name), SettingsKeys::FlowControl, m_flowControl);
    //settings.set(QString(::Name), SettingsKeys::AutoReconnect, m_autoReconnect);
  }

  void SerialConnector::load()
  {
    const auto& settings = Settings::getInstance();

    const auto port = settings.get(QString(::Name), SettingsKeys::Port, SerialConnector::UnknownPort).toString();
    if(isPortNameCorrect(port) && isPortAvailable(port))
    {
      setPort(port);
    }
    else if (m_portsModel->rowCount() > 0)
    {
      setPort(m_portsModel->data(m_portsModel->index(0), BasicQMLModel::ValueRole).toString());
    }
    else
    {
      setPort(SerialConnector::UnknownPort);
    }

    m_baudrate      = settings.getEnum(QString(::Name), SettingsKeys::Baudrate, SerialConnector::BaudRate::Baud57600);
    m_dataBits      = settings.getEnum(QString(::Name), SettingsKeys::Databits, SerialConnector::DataBits::Data8);
    m_parity        = settings.getEnum(QString(::Name), SettingsKeys::Parity, SerialConnector::Parity::NoParity);
    m_stopBits      = settings.getEnum(QString(::Name), SettingsKeys::StopBits, SerialConnector::StopBits::OneStop);
    m_flowControl   = settings.getEnum(QString(::Name), SettingsKeys::FlowControl, SerialConnector::FlowControl::NoFlowControl);
    m_autoReconnect = settings.get(QString(::Name), SettingsKeys::AutoReconnect, true).toBool();
  }

  void SerialConnector::write(const QByteArray& data)
  {
    if(m_worker && state() == State::Connected)
    {
      emit send(data);
    }
  }

  void SerialConnector::requestConnect()
  {
    if(m_worker)
    {
      return;
    }

    if(m_port.isEmpty())
    {
      emit connectionFailed(tr("No ports available for connection"));
      return;
    }

    setState(ConnectMethodBase::State::Connecting);
    m_workerThread = new QThread{ this };
    m_worker       = new SerialWorker{ m_port };
    m_worker->moveToThread(m_workerThread);

    m_worker->setBaudRate(static_cast<SerialWorker::BaudRate>(m_baudrate));
    m_worker->setFlowControl(static_cast<SerialWorker::FlowControl>(m_flowControl));
    m_worker->setParity(static_cast<SerialWorker::Parity>(m_parity));
    m_worker->setDataBits(static_cast<SerialWorker::DataBits>(m_dataBits));
    m_worker->setStopBits(static_cast<SerialWorker::StopBits>(m_stopBits));

    QObject::connect(m_workerThread, &QThread::started,
                           m_worker, &SerialWorker::openPort);
    QObject::connect(m_workerThread, &QThread::finished,
                           m_worker, &SerialWorker::deleteLater);

    QObject::connect(m_worker, &SerialWorker::connectionSuccess,
                         this, &SerialConnector::onConnected);
    QObject::connect(m_worker, &SerialWorker::connectionFailed,
                         this, &SerialConnector::onFailedToConnect);
    QObject::connect(m_worker, &SerialWorker::connectionClosed,
                         this, &SerialConnector::stop);

    m_workerThread->start();
  }

  void SerialConnector::requestDisconnect()
  {
    QMetaObject::invokeMethod(m_worker, &SerialWorker::closePort, Qt::QueuedConnection);
  }

  void SerialConnector::requestReconnect()
  {
    requestConnect();
  }

  BasicQMLModel *SerialConnector::portsModel() const
  {
    return m_portsModel;
  }

  void SerialConnector::onConnected()
  {
    if(state() != State::Reconnecting)
    {
      QObject::connect(this, &SerialConnector::send,
                       m_worker, &SerialWorker::formatAndSend,
                       Qt::QueuedConnection);
      QObject::connect(m_worker, &SerialWorker::errorOccurred,
                       this, &SerialConnector::onErrorOccured);
    }
    setState(State::Connected);
  }

  void SerialConnector::onFailedToConnect()
  {
    const auto errorText = QStringLiteral("%1 - %2").arg(m_port, m_worker->errorString());
    emit connectionFailed(errorText);
    stop();
  }

  void SerialConnector::stop()
  {
    if(m_worker)
    {
      m_workerThread->exit();
      m_workerThread->wait();

      m_worker = nullptr;
      delete m_workerThread;
      m_workerThread = nullptr;
    }

    setState(State::Idle);
  }

  void SerialConnector::onErrorOccured(const QSerialPort::SerialPortError& error)
  {
    if(error == QSerialPort::SerialPortError::NoError)
      return;
    if(state() == State::Reconnecting)
      return;

    if(autoReconnect())
    {
      setState(State::Reconnecting);
      QMetaObject::invokeMethod(m_worker, &SerialWorker::reconnect, Qt::QueuedConnection);
    }
    else
    {
      requestDisconnect();
    }
  }
}
