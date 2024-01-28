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

#pragma once

#include "v2cbase.h"
#include <QtQml/qqmlregistration.h>

namespace vol2com
{
  class WorkModeBase : public V2CBase
  {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString qmlDelegate READ qmlDelegate CONSTANT)

  public:
    using V2CBase::V2CBase;

    static QString qmlDelegatesPath()
    {
      return QStringLiteral("qrc:/qt/qml/vol2com/qml/");
    }

    virtual QString name() const = 0;
    virtual QString qmlDelegate() const = 0;

  public slots:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void process() = 0;

  signals:
    void dataReady(const QByteArray& data);
  };
}
