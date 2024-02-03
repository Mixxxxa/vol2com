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

#ifndef CONNECTPAGEVIEWMODEL_H
#define CONNECTPAGEVIEWMODEL_H

#include <QObject>
#include "connectmethodbase.h"
#include "controller.h"
#include <QtQml/qqmlregistration.h>

namespace vol2com
{
    class ConnectPageViewModel : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(QString qmlDelegate READ qmlDelegate NOTIFY qmlDelegateChanged)
        Q_PROPERTY(vol2com::Controller::ConnectType connectType READ connectType WRITE setConnectType NOTIFY connectTypeChanged)
        Q_PROPERTY(vol2com::ConnectMethodBase::State state READ state NOTIFY stateChanged)

    public:
        ConnectPageViewModel(QObject* parent = nullptr);

        vol2com::ConnectMethodBase::State state() const { return m_state; }
        vol2com::Controller::ConnectType connectType() const { return m_connectType; }
        QString qmlDelegate() const { return m_qmlDelegate; }

    public slots:
        void setConnectType(vol2com::Controller::ConnectType connectType);

    signals:
        void stateChanged(vol2com::ConnectMethodBase::State state);
        void connectTypeChanged(vol2com::Controller::ConnectType connectType);
        void qmlDelegateChanged(QString qmlDelegate);

    private:
        Controller& m_controller;
        vol2com::ConnectMethodBase::State m_state;
        vol2com::Controller::ConnectType m_connectType;
        QString m_qmlDelegate;

    private slots:
        void onConnectorChanged(std::shared_ptr<ConnectMethodBase> connector);
        void setState(vol2com::ConnectMethodBase::State state);
        void setQmlDelegate(QString qmlDelegate);
    };
}

#endif // CONNECTPAGEVIEWMODEL_H
