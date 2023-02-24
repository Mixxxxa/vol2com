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

#include "toolbarviewmodel.h"

namespace vol2com
{
ToolbarViewModel::ToolbarViewModel(QObject *parent) :
    QObject(parent),
    m_showStatus(false),
    m_backarrowActive(false)
{
    auto& controller = Controller::getInstance();
    setPage(controller.page());
    onModeChanged(controller.mode());

    QObject::connect(&controller, &Controller::pageChanged,
                     this, &ToolbarViewModel::setPage);
    QObject::connect(&controller, &Controller::modeChanged,
                     this, &ToolbarViewModel::onModeChanged);
    QObject::connect(this, &ToolbarViewModel::pageChanged,
                     this, &ToolbarViewModel::updateBackArrow);
}

void ToolbarViewModel::setPage(vol2com::Controller::Page page)
{
    if (m_page == page)
        return;

    m_page = page;
    emit pageChanged(m_page);
}

void ToolbarViewModel::setShowStatus(bool showStatus)
{
    if (m_showStatus == showStatus)
        return;

    m_showStatus = showStatus;
    emit showStatusChanged(m_showStatus);
}

void ToolbarViewModel::setBackarrowActive(bool backarrowActive)
{
    if (m_backarrowActive == backarrowActive)
        return;

    m_backarrowActive = backarrowActive;
    emit backarrowActiveChanged(m_backarrowActive);
}

void ToolbarViewModel::updateBackArrow()
{
    bool active = (!m_workMode.expired()) && (m_page == Controller::Page::WorkModePage);
    setBackarrowActive(active);
}

void ToolbarViewModel::onModeChanged(std::shared_ptr<WorkModeBase> mode)
{
    m_workMode = mode;
    updateBackArrow();
}
}


