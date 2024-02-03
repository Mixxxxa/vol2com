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

//#ifndef UIVIEWMODEL_H
//#define UIVIEWMODEL_H
#pragma once

#include <QObject>
#include "controller.h"

#include <QtQml/qqmlregistration.h>
//#include <qqml.h>

namespace vol2com
{
  class ToolbarViewModel : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(vol2com::Controller::Page page READ page WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(bool showStatus READ showStatus WRITE setShowStatus NOTIFY showStatusChanged)
    Q_PROPERTY(bool backarrowActive READ backarrowActive WRITE setBackarrowActive NOTIFY backarrowActiveChanged)
    QML_ELEMENT

  public:
    explicit ToolbarViewModel(QObject *parent = nullptr);

    Controller::Page page() const { return m_page; }
    bool showStatus() const { return m_showStatus; }
    bool backarrowActive() const { return m_backarrowActive; }

  public slots:
    void setPage(vol2com::Controller::Page page);
    void setShowStatus(bool showStatus);
    void setBackarrowActive(bool backarrowActive);
    void onModeChanged(std::shared_ptr<WorkModeBase> mode);

  signals:
    void pageChanged(Controller::Page page);
    void showStatusChanged(bool showStatus);
    void backarrowActiveChanged(bool backarrowActive);

  private:
    void updateBackArrow();

    Controller::Page m_page = Controller::Page::ConnectPage;
    bool m_showStatus = false;
    bool m_backarrowActive = false;
    std::weak_ptr<WorkModeBase> m_workMode;
  };
}

//#endif // UIVIEWMODEL_H
