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

#include "controller.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

#include "settings.h"
#include "serial.h"
#include "basslibwrapper.h"
#include "equalizer.h"
#include "guihelper.h"
#include "appstyle.h"
#include "systemevents.h"

#include "datamodels/basicqmlmodel.h"
#include "workmodesfactory.h"

#include "workmodes/workmodebase.h"
#include "workmodes/generalmode.h"
#include "workmodes/manualmode.h"
#include "workmodes/fademode.h"
#include "workmodes/christmasmode.h"
#include "workmodes/randommode.h"

#include <QDebug>

using namespace vol2com;

Controller::Controller() :
    V2CBase(nullptr),
    m_connectType(ConnectType::Serial),
    m_page(Page::WorkModePage),
    m_mainWindow(nullptr),
    m_connector(nullptr),
    m_bassLib(nullptr),
    m_equalizer(nullptr),
    m_factory(nullptr),
    m_mode(nullptr),
    m_engine(nullptr),
    m_systemEvents(nullptr)
{
}

Controller::~Controller()
{
    deInit();
}

bool Controller::isMainWindowOpened() const
{
    return (m_mainWindow && m_mainWindow->isActive());
}

Controller& Controller::getInstance()
{
    static Controller instance;
    return instance;
}

void Controller::init()
{
    GUIHelper::registerMetaTypes();
    m_connector = std::make_shared<Serial>();
    m_bassLib = std::make_shared<BassLibWrapper>();
    m_factory = std::make_shared<WorkModesFactory>();
    m_equalizer = std::make_shared<Equalizer>();
    m_systemEvents = std::make_unique<SystemEvents>();

    m_factory->add<GeneralMode>("General", tr("General"), tr("All color spectre available"), "qrc:/res/modes/general.svg");
    m_factory->add<FadeMode>("Fade", tr("Fade"), tr("Fading of selected color"), "qrc:/res/modes/fade.svg");
    m_factory->add<RandomMode>("Random", tr("Random"), tr("Randomly changing colors"), "qrc:/res/modes/random.svg");
    m_factory->add<ManualMode>("Manual", tr("Manual"), tr("Select any color"), "qrc:/res/modes/manual.svg");
    m_factory->add<ChristmasMode>("Christmas", tr("Christmas"), tr("Imitation of christmas leds"), "qrc:/res/modes/christmas.svg");
    //m_factory->add<GeneralMode>("Party", tr("Party"), tr("Blue + Purple"), "qrc:/res/modes/party.svg");
    //m_factory->add<GeneralMode>("Gaming", tr("Gaming"), tr("Red accents"), "qrc:/res/modes/gaming.svg");
    //m_factory->add<GeneralMode>("Fireplace", tr("Fireplace"), tr("Fireplace imitation"), "qrc:/res/modes/fireplace.svg");

    QObject::connect(connector().get(), &ConnectMethodBase::stateChanged,
                     this, &Controller::onConnectionStateChanged);
    QObject::connect(qGuiApp, &QGuiApplication::aboutToQuit,
                     this, &Controller::deInit);
    QObject::connect(connector().get(), &ConnectMethodBase::connectionFailed, [this](const QString& message)
    {
        if(isMainWindowOpened())
            showMessage(tr("Connection failed"), message);
        else
            m_systemEvents->sendConnectionFailedToast(message);
    });
    QObject::connect(bassLib().get(), &BassLibWrapper::showNotification, [this](const QString& title, const QString& message)
    {
        if(isMainWindowOpened())
            showMessage(title, message);
        else
            m_systemEvents->sendDeviceChangedToast(title, message);
    });
}

void Controller::deInit()
{
    closeUI();

    if(m_engine)
        m_engine = nullptr;
    if(m_mode)
        m_mode = nullptr;
    if(m_connector)
        m_connector = nullptr;
    if(m_equalizer)
        m_equalizer = nullptr;
    if(m_bassLib)
        m_bassLib = nullptr;
    if(m_factory)
        m_factory = nullptr;
    if(m_systemEvents)
        m_systemEvents = nullptr;
}

void Controller::openUI()
{
    if(!m_engine)
    {
        m_engine = std::make_unique<QQmlApplicationEngine>();
        m_engine->addImportPath("qrc:/qml/");
        vol2com::GUIHelper::registerQMLTypes();
        qmlRegisterSingletonInstance("vol2com", 1, 0, "Settings", &Settings::getInstance());
        qmlRegisterSingletonInstance("vol2com", 1, 0, "Controller", this);
        qmlRegisterSingletonInstance("vol2com", 1, 0, "AppStyle", &AppStyle::getInstance());
        QObject::connect(&Settings::getInstance(), &Settings::languageChanged, [this]()
        {
            showMessage(tr("Language changed"),
                        tr("Changes will come into effect when you restart the app"));
        });
    }

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(m_engine.get(), &QQmlApplicationEngine::objectCreated,
                     qGuiApp, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    m_engine->load(url);
    m_mainWindow = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
    if(m_mainWindow)
    {
        QObject::connect(m_mainWindow, &QQuickWindow::windowStateChanged,
                         this, &Controller::windowStateChanged);
    }
}

void Controller::closeUI()
{
    if(!m_engine || m_engine->rootObjects().isEmpty())
        return;

    if(m_mainWindow)
    {
        m_mainWindow->close();
        m_mainWindow->releaseResources();
        m_mainWindow->deleteLater();
        m_mainWindow = nullptr;
    }

    if(m_engine)
    {
        m_engine->collectGarbage();
        m_engine->clearComponentCache();
        qGuiApp->processEvents();
    }
}

void Controller::exit()
{
    Settings::getInstance().save();
    qGuiApp->exit(0);
}

void Controller::openGUIPage(const Controller::Page& page)
{
    setPage(page);
    if(m_mainWindow)
    {
        if(m_mainWindow->visibility() == QWindow::Minimized)
            m_mainWindow->showNormal();
        if(!m_mainWindow->isActive())
            m_mainWindow->requestActivate();
    }
}

void Controller::retryConnect()
{
    if(m_connector)
        m_connector->reconnect();
}

void Controller::setConnectType(Controller::ConnectType connectType)
{
    if(connectType == ConnectType::WebSocket)
    {
        emit showMessage("Websocket", tr("At the moment WebSocket is not implemented. Sorry."));
        emit connectTypeChanged(ConnectType::Serial);
        return;
    }

    if (m_connectType == connectType)
        return;
    m_connectType = connectType;
    emit connectTypeChanged(m_connectType);
}

void Controller::setMode(const QString& modename)
{
    if(m_mode)
    {
        if(m_mode->name() == modename)
            return;

        m_mode->stop();
        m_mode->disconnect();
    }

    m_mode = m_factory->create(modename);
    if(m_mode != nullptr)
    {
        if(m_connector)
        {
            QObject::connect(m_mode.get(), &WorkModeBase::dataReady,
                             m_connector.get(), &ConnectMethodBase::write);

            if(m_connector->state() == ConnectMethodBase::State::Connected)
                m_mode->start();
        }
    }
    else
    {
        m_bassLib->stop();
    }

    emit modeChanged(m_mode);
}

void Controller::clearMode()
{
    setMode("");
}

void Controller::onConnectionStateChanged(const ConnectMethodBase::State& state)
{
    switch (state) {
    case ConnectMethodBase::State::Idle:
        qDebug() << "Idle";
        break;
    case ConnectMethodBase::State::Connecting:
        qDebug() << "Connecting";
        break;
    case ConnectMethodBase::State::Connected:
        qDebug() << "Connected";
        break;
    case ConnectMethodBase::State::Reconnecting:
        qDebug() << "Reconnecting";
        break;
    }

    switch(state){
    case ConnectMethodBase::State::Connected:
        qDebug() << "Start";
        if(m_mode != nullptr)
        {
            //QObject::connect(m_mode.get(), &WorkModeBase::dataReady,
            //                 m_connector.get(), &ConnectMethodBase::write, Qt::UniqueConnection);
            m_mode->start();
        }
        break;
    default:
        qDebug() << "Stop";
        if(m_mode)
            m_mode->stop();
    }
}

void Controller::setPage(Controller::Page page)
{
    if (m_page == page)
        return;

    m_page = page;
    emit pageChanged(m_page);
}
