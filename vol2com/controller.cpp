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
#include <QJSEngine>
#include <QTimer>

#include "settings.h"
#include "serialconnector.h"
#include "basslibwrapper.h"
#include "equalizer.h"
#include "traymenu.h"
//#include "guihelper.h"
//#include "appstyle.h"
//#include "systemevents.h"

//#include "datamodels/basicqmlmodel.h"
#include "v2cbase.h"
#include "workmodesfactory.h"

#include "workmodes/workmodebase.h"
#include "workmodes/generalmode.h"
#include "workmodes/manualmode.h"
#include "workmodes/fademode.h"
#include "workmodes/randommode.h"
#include "workmodes/aimode.h"

#include <QDebug>

namespace vol2com
{
  Controller::Controller(QObject *parent)
    : V2CBase     { parent }
    , m_connector { std::make_shared<SerialConnector>() }
    , m_bassLib   { std::make_shared<BassLibWrapper>() }
    , m_equalizer { std::make_shared<Equalizer>() }
    , m_factory   { std::make_shared<WorkModesFactory>() }
    , m_tray      { std::make_unique<TrayMenu>() }
  {
    m_connector->load();
    m_equalizer->load();

    using namespace Qt::Literals::StringLiterals;
    m_factory->add<GeneralMode>(u"General"_s,     tr("General"),   tr("All color spectre available"), u"qrc:/qt/qml/vol2com/res/modes/general.svg"_s);
    m_factory->add<FadeMode>(u"Fade"_s,           tr("Fade"),      tr("Fading of selected color"),    u"qrc:/qt/qml/vol2com/res/modes/fade.svg"_s);
    m_factory->add<RandomMode>(u"Random"_s,       tr("Random"),    tr("Randomly changing colors"),    u"qrc:/qt/qml/vol2com/res/modes/random.svg"_s);
    m_factory->add<ManualMode>(u"Manual"_s,       tr("Manual"),    tr("Select any color"),            u"qrc:/qt/qml/vol2com/res/modes/manual.svg"_s);
    m_factory->add<AiMode>(u"AI"_s,               tr("AI"),        tr("AI driven selected colors"),   u"qrc:/qt/qml/vol2com/res/modes/ai.svg"_s);

    QObject::connect(connector().get(), &ConnectMethodBase::stateChanged,
                     this, &Controller::onConnectionStateChanged);
    QObject::connect(qGuiApp, &QGuiApplication::aboutToQuit,
                     this, &Controller::deInit);
    QObject::connect(connector().get(), &ConnectMethodBase::connectionFailed,
                     [this](const QString& message)
                     {
                       if(isMainWindowOpened())
                       {
                         showMessage(tr("Connection failed"), message);
                       }
                       //else
                       //    m_systemEvents->sendConnectionFailedToast(message);
                     });
    QObject::connect(bassLib().get(), &BassLibWrapper::showNotification,
                     [this](const QString& title, const QString& message)
                     {
                       if(isMainWindowOpened())
                       {
                         showMessage(title, message);
                       }
                       //else
                       //    m_systemEvents->sendDeviceChangedToast(title, message);
                     });
    QObject::connect(&Settings::getInstance(), &Settings::languageChanged,
                     this, [this]()
                     {
                       showMessage(tr("Language changed"),
                                   tr("Changes will come into effect when you restart the app"));
                     });

    QObject::connect(m_tray.get(), &TrayMenu::ShowUI, this, []()
                     {

                       qDebug() << "Open UI";
                     });

    QObject::connect(m_tray.get(), &TrayMenu::Exit,
                             this, &Controller::exit);
  }

  Controller::~Controller()
  {
    deInit();
  }

  bool Controller::isMainWindowOpened() const
  {
    return m_mainWindow
      && m_mainWindow->isActive();
  }

  Controller *Controller::create(QQmlEngine *, QJSEngine */*engine*/)
  {
    Controller *result = &(Controller::getInstance());
    QJSEngine::setObjectOwnership(result, QJSEngine::ObjectOwnership::CppOwnership);
    return result;
  }

  Controller& Controller::getInstance()
  {
    static Controller instance;
    return instance;
  }

  void Controller::deInit()
  {
    closeUI();

    if(m_engine)
    {
      m_engine = nullptr;
    }

    if(m_mode)
    {
      m_mode->save();
      m_mode = nullptr;
    }

    if(m_connector)
    {
      m_connector->save();
      m_connector = nullptr;
    }

    if(m_equalizer)
    {
      m_equalizer->save();
      m_equalizer = nullptr;
    }

    if(m_bassLib)
    {
      m_bassLib = nullptr;
    }

    if(m_factory)
    {
      m_factory = nullptr;
    }
  }

  void Controller::openUI()
  {
    if(!m_engine)
    {
      m_engine = std::make_unique<QQmlApplicationEngine>();

      constexpr auto ForceExit = []()
      {
        QCoreApplication::exit(-1);
      };

      QObject::connect(m_engine.get(), &QQmlApplicationEngine::objectCreationFailed,
                       qGuiApp, ForceExit, Qt::QueuedConnection);
    }


    const QUrl url(u"qrc:/qt/qml/vol2com/qml/main.qml"_qs);
    m_engine->load(url);

    if(auto mainWindow = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first()))
    {
      m_mainWindow = mainWindow;
      QObject::connect(m_mainWindow, &QQuickWindow::windowStateChanged,
                       this, &Controller::windowStateChanged);
    }
  }

  void Controller::closeUI()
  {
    if(!m_engine || m_engine->rootObjects().isEmpty())
    {
      return;
    }

    if(m_mainWindow)
    {
      m_mainWindow->close();
      m_mainWindow->releaseResources();
      m_mainWindow->deleteLater();
      m_mainWindow = nullptr;
    }

    m_engine->collectGarbage();
    m_engine->clearComponentCache();
    qGuiApp->processEvents();
  }

  void Controller::exit()
  {
    Settings::getInstance().save();
    QTimer::singleShot(0, qGuiApp, []()
                       {
                         qGuiApp->exit(0);
                       });
  }

  void Controller::RequestExit()
  {

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
    {
      m_connector->requestReconnect();
    }
  }

  void Controller::setConnectType(ConnectType connectType)
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
      {
        return;
      }

      m_mode->stop();
      m_mode->disconnect();
    }

    m_mode = m_factory->create(modename);
    if(m_mode)
    {
      m_mode->load();
      if(m_connector)
      {
        QObject::connect(     m_mode.get(), &WorkModeBase::dataReady,
                         m_connector.get(), &ConnectMethodBase::write);

        if(m_connector->state() == ConnectMethodBase::State::Connected)
        {
          m_mode->start();
        }
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
      if(m_mode)
      {
        QObject::connect(     m_mode.get(), &WorkModeBase::dataReady,
                         m_connector.get(), &ConnectMethodBase::write, Qt::UniqueConnection);
        m_mode->start();
      }
      break;
    default:
      qDebug() << "Stop";
      if(m_mode)
        m_mode->stop();
    }
  }

  void Controller::setPage(Page page)
  {
    if (m_page == page)
      return;

    m_page = page;
    emit pageChanged(m_page);
  }
}
