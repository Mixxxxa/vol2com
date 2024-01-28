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

#ifndef FADEMODE_H
#define FADEMODE_H

#include "workmodewithselector.h"
#include "utils/boundedvalue.h"

namespace vol2com
{
  class FadeMode : public WorkModeWithSelector
  {
    Q_OBJECT
    Q_PROPERTY(vol2com::BoundedValue* hue READ hue CONSTANT)

  public:
    explicit FadeMode(QObject *parent = nullptr);

    virtual QString name() const override;
    virtual QString qmlDelegate() const override;
    virtual QColor getGuiColor(uint8_t value) const override;
    BoundedValue *hue();

  public slots:
    virtual void process() override;
    virtual void save() override;
    virtual void load() override;

  private:
    vol2com::BoundedValue m_hue;
  };
}

#endif // FADEMODE_H
