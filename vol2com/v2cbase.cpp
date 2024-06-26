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

#include "v2cbase.h"
#include <QTimer>

using namespace std::chrono_literals;

namespace vol2com
{
  V2CBase::V2CBase(QObject *parent)
    : QObject      { parent }
    , m_saveTimer  { new QTimer{ this } }
    , m_saveTimeout{ 5s }
  {
    m_saveTimer->setSingleShot(true);
    m_saveTimer->setTimerType(Qt::VeryCoarseTimer);
    QObject::connect(m_saveTimer, &QTimer::timeout,
                            this, &V2CBase::save);
  }

  void V2CBase::scheduleSave()
  {
    if(m_saveTimer)
    {
      m_saveTimer->start(m_saveTimeout);
    }
  }

  std::chrono::milliseconds V2CBase::saveTimeout() const
  {
    return m_saveTimeout;
  }

  void V2CBase::setSaveTimeout(std::chrono::milliseconds timeout)
  {
    m_saveTimeout = timeout;
  }
}
