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

#ifndef CONNECTMETHODBASE_H
#define CONNECTMETHODBASE_H

#include "v2cbase.h"

namespace vol2com
{
    class ConnectMethodBase : public V2CBase
    {
        Q_OBJECT

        Q_PROPERTY(QString qmlDelegate READ qmlDelegate CONSTANT)
        Q_PROPERTY(vol2com::ConnectMethodBase::State state READ state NOTIFY stateChanged)

    public:
        enum class State
        {
            Idle,
            Connecting,
            Connected,
            Reconnecting
        };
        Q_ENUM(State)

        explicit ConnectMethodBase(QObject *parent = nullptr) :
            V2CBase(parent),
            m_state(State::Idle)
        {}
        virtual ~ConnectMethodBase() {}
        virtual QString qmlDelegate() const = 0;
        static QString qmlDelegatesPath()
        {
            return QStringLiteral("qrc:/qml/vol2com/Views/Connect/");
        }

        State state() const { return m_state; }

    signals:
        void stateChanged(ConnectMethodBase::State state);
        void connectionFailed(const QString& message);

    public slots:
        virtual void write(const QByteArray& data) = 0;
        virtual void connectDevice() = 0;
        virtual void disconnectDevice() = 0;
        virtual void reconnect() = 0;

    protected:
        void setState(ConnectMethodBase::State state)
        {
            if (m_state == state)
                return;

            m_state = state;
            emit stateChanged(m_state);
        }

        State m_state;
    };
}

Q_DECLARE_METATYPE( vol2com::ConnectMethodBase* )
Q_DECLARE_METATYPE( vol2com::ConnectMethodBase::State )

#endif // CONNECTMETHODBASE_H
