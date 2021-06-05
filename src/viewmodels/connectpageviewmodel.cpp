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

#include "connectpageviewmodel.h"

using namespace vol2com;

ConnectPageViewModel::ConnectPageViewModel(QObject* parent) :
    QObject(parent),
    m_controller(Controller::getInstance()),
    m_state(ConnectMethodBase::State::Idle),
    m_connectType(m_controller.connectType()),
    m_qmlDelegate("")
{
    onConnectorChanged(m_controller.connector());
    QObject::connect(&m_controller, &Controller::connectorChanged,
                     this, &ConnectPageViewModel::onConnectorChanged);
    QObject::connect(&m_controller, &Controller::connectTypeChanged,
                     this, &ConnectPageViewModel::setConnectType);
    QObject::connect(this, &ConnectPageViewModel::connectTypeChanged,
                     &m_controller, &Controller::setConnectType);
}

void ConnectPageViewModel::setConnectType(vol2com::Controller::ConnectType connectType)
{
    if (m_connectType == connectType)
        return;

    m_connectType = connectType;
    emit connectTypeChanged(m_connectType);
}

void ConnectPageViewModel::setState(ConnectMethodBase::State state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}

void ConnectPageViewModel::setQmlDelegate(QString qmlDelegate)
{
    if (m_qmlDelegate == qmlDelegate)
        return;

    m_qmlDelegate = qmlDelegate;
    emit qmlDelegateChanged(m_qmlDelegate);
}

void ConnectPageViewModel::onConnectorChanged(std::shared_ptr<ConnectMethodBase> connector)
{
    if(connector == nullptr)
    {
        setQmlDelegate("");
        setState(ConnectMethodBase::State::Idle);
    }
    else
    {
        setQmlDelegate(connector->qmlDelegate());
        setState(connector->state());
        QObject::connect(connector.get(), &ConnectMethodBase::stateChanged,
                         this, &ConnectPageViewModel::setState);
    }
}
