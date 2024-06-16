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

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "v2cbase.h"
#include "connectmethodbase.h"
#include <memory>
#include <QtQml/qqmlregistration.h>

class QQmlApplicationEngine;
class QQuickWindow;
class QQmlEngine;
class QJSEngine;

namespace vol2com
{
  //class BasicQMLModel;
  class Equalizer;
  class BassLibWrapper;
  //class WorkModesModel;
  class WorkModesFactory;
  class WorkModeBase;
  class TrayMenu;
  //class SystemEvents;

  class Controller : public V2CBase
  {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(vol2com::Controller::ConnectType connectType READ connectType WRITE setConnectType NOTIFY connectTypeChanged)
    Q_PROPERTY(vol2com::Controller::Page page READ page WRITE setPage NOTIFY pageChanged)

  public:
    enum class ConnectType
    {
      Serial,
      WebSocket
    };
    Q_ENUM(ConnectType)

    enum class Page
    {
      ConnectPage,
      WorkModePage,
      EqualizerPage,
      RemoteCommandsPage,
      SettingsPage,
      AboutPage
    };
    Q_ENUM(Page)

    static Controller *create(QQmlEngine *, QJSEngine *);
    static Controller& getInstance();
    ~Controller();

    bool isMainWindowOpened() const;
    ConnectType connectType() const { return m_connectType; }
    Page page() const { return m_page; }

    std::shared_ptr<ConnectMethodBase> connector() const { return m_connector; }
    std::shared_ptr<BassLibWrapper> bassLib() const { return m_bassLib; }
    std::shared_ptr<Equalizer> equalizer() const { return m_equalizer; }
    std::shared_ptr<WorkModesFactory> factory() const { return m_factory; }
    std::shared_ptr<WorkModeBase> mode() const { return m_mode; }

  public slots:
    void deInit();
    virtual void save() override {}
    virtual void load() override {}

    void openUI();
    void closeUI();
    void exit();
    void RequestExit();

    void openGUIPage(const vol2com::Controller::Page& page);
    void retryConnect();

    void setConnectType(vol2com::Controller::ConnectType connectType);
    void setPage(vol2com::Controller::Page page);
    void setMode(const QString& modename);
    void clearMode();
    void onConnectionStateChanged(const ConnectMethodBase::State& state);

  signals:
    void connectTypeChanged(vol2com::Controller::ConnectType connectType);
    void pageChanged(Controller::Page page);
    void connectorChanged(std::shared_ptr<ConnectMethodBase> connector);
    void modeChanged(std::shared_ptr<WorkModeBase> mode);
    void showMessage(const QString& title, const QString& text);
    void windowStateChanged(const Qt::WindowState& state);

  private:
    Controller(QObject *parent = nullptr);
    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;

    ConnectType m_connectType = ConnectType::Serial;
    Controller::Page m_page = Controller::Page::ConnectPage;

    QQuickWindow* m_mainWindow = nullptr;
    std::shared_ptr<ConnectMethodBase> m_connector;
    std::shared_ptr<BassLibWrapper> m_bassLib;
    std::shared_ptr<Equalizer> m_equalizer;
    std::shared_ptr<WorkModesFactory> m_factory;
    std::shared_ptr<WorkModeBase> m_mode;
    std::unique_ptr<QQmlApplicationEngine> m_engine;

    std::unique_ptr<TrayMenu> m_tray;
    //std::unique_ptr<SystemEvents> m_systemEvents;
  };
}

//Q_DECLARE_METATYPE( vol2com::Controller::ConnectType )
//Q_DECLARE_METATYPE( vol2com::Controller::Page )

#endif // CONTROLLER_H
