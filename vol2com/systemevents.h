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

#ifndef SYSTEMEVENTS_H
#define SYSTEMEVENTS_H

#include <QObject>
#include <memory>

namespace vol2com
{
    class RetryToastHandler;
    class SwitchDeviceHandler;

    class SystemEvents : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemEvents(QObject *parent = nullptr);
        ~SystemEvents();

    public slots:
        void sendConnectionFailedToast(const QString& text);
        void sendDeviceChangedToast(const QString& title, const QString& text);

    private:
        bool setupNotifications();

        std::shared_ptr<RetryToastHandler> m_failedToastHandler;
        std::shared_ptr<SwitchDeviceHandler> m_deviceChangedToastHandler;
    };
}

#endif // SYSTEMEVENTS_H
