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

#include "systemevents.h"
#include <QCoreApplication>
#include "settings.h"
#include "controller.h"
#include "libs/WinToast/wintoastlib.h"

namespace vol2com
{
    class RetryToastHandler : public WinToastLib::IWinToastHandler {
    public:
        RetryToastHandler() : WinToastLib::IWinToastHandler() { }
        virtual ~RetryToastHandler() { }

        virtual void toastActivated() const override
        {
            QMetaObject::invokeMethod(&Controller::getInstance(),
                                      "openGUIPage",
                                      Qt::QueuedConnection,
                                      Q_ARG(vol2com::Controller::Page, vol2com::Controller::Page::ConnectPage));
        }

        virtual void toastActivated(int actionIndex) const override
        {
            if(actionIndex == 0)
            {
                QMetaObject::invokeMethod(&Controller::getInstance(),
                                          &Controller::retryConnect,
                                          Qt::QueuedConnection);
            }
        }

        virtual void toastDismissed(WinToastDismissalReason state) const override
        {
            Q_UNUSED(state)
        };

        virtual void toastFailed() const override {}
    };

    class SwitchDeviceHandler : public WinToastLib::IWinToastHandler {
    public:
        SwitchDeviceHandler() : WinToastLib::IWinToastHandler() { }
        virtual ~SwitchDeviceHandler() { }

        virtual void toastActivated() const override
        {
            QMetaObject::invokeMethod(&Controller::getInstance(),
                                      "openGUIPage",
                                      Qt::QueuedConnection,
                                      Q_ARG(vol2com::Controller::Page, vol2com::Controller::Page::WorkModePage));
        }

        virtual void toastActivated(int actionIndex) const override
        {
            Q_UNUSED(actionIndex);
        }

        virtual void toastDismissed(WinToastDismissalReason state) const override
        {
            Q_UNUSED(state)
        };

        virtual void toastFailed() const override {}
    };
}

using namespace vol2com;
using namespace WinToastLib;

SystemEvents::SystemEvents(QObject *parent) :
    QObject(parent),
    m_failedToastHandler(nullptr),
    m_deviceChangedToastHandler(nullptr)
{
    const bool notificationsAvailable = WinToast::isCompatible() && setupNotifications();
    Settings::getInstance().setNotificationsAvailable(notificationsAvailable);
}

SystemEvents::~SystemEvents()
{
    WinToast::instance()->clear();
    if(m_failedToastHandler)
    {
        m_failedToastHandler = nullptr;
    }
}

void SystemEvents::sendConnectionFailedToast(const QString& text)
{
    WinToastTemplate templ = WinToastTemplate(WinToastTemplate::Text02);
    templ.setTextField(tr("Connection failed").toStdWString(), WinToastTemplate::FirstLine);
    templ.setTextField(text.toStdWString(), WinToastTemplate::SecondLine);
    templ.setDuration(WinToastTemplate::Duration::System);
    templ.addAction(tr("Retry").toStdWString());

    if(!m_failedToastHandler)
    {
        m_failedToastHandler = std::make_shared<RetryToastHandler>();
    }

    WinToast::instance()->showToast(templ, m_failedToastHandler);
}

void SystemEvents::sendDeviceChangedToast(const QString& title, const QString& text)
{
    WinToastTemplate templ = WinToastTemplate(WinToastTemplate::Text02);
    templ.setTextField(title.toStdWString(), WinToastTemplate::FirstLine);
    templ.setTextField(text.toStdWString(), WinToastTemplate::SecondLine);
    templ.setDuration(WinToastTemplate::Duration::System);

    if(!m_deviceChangedToastHandler)
    {
        m_deviceChangedToastHandler = std::make_shared<SwitchDeviceHandler>();
    }

    WinToast::instance()->showToast(templ, m_deviceChangedToastHandler);
}

bool SystemEvents::setupNotifications()
{
    const auto AppName = QCoreApplication::applicationName();
    const auto Developer = "mixxxxa";
    const auto AppId = QString("%1.%2").arg(Developer, AppName);
    auto toast = WinToast::instance();
    toast->setAppName(AppName.toStdWString());
    toast->setAppUserModelId(AppId.toStdWString());
    if (!WinToast::instance()->initialize())
    {
        return false;
    }

    return true;
}
