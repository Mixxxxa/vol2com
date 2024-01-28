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

#include "manualmode.h"
#include "qnamespace.h"
#include "settings.h"
#include "utils/math.hpp"

namespace vol2com
{
  ManualMode::ManualMode(QObject *parent)
    : WorkModeBase{ parent }
    , m_hue       { 0, 359, 0,   BoundedValue::EOverflowBehavior::Clamp }
    , m_saturation{ 0, 255, 255, BoundedValue::EOverflowBehavior::Clamp }
    , m_value     { 0, 255, 255, BoundedValue::EOverflowBehavior::Clamp }
  {
    m_color.setHsv(m_hue.value(), m_saturation.value(), m_value.value());
    m_refColor.setHsv(m_hue.value(), 255, 255);
    QObject::connect(&m_hue, &BoundedValue::valueChanged,
                     this, &ManualMode::onHsvChanged);
    QObject::connect(&m_saturation, &BoundedValue::valueChanged,
                     this, &ManualMode::onHsvChanged);
    QObject::connect(&m_value, &BoundedValue::valueChanged,
                     this, &ManualMode::onHsvChanged);
  }

  void ManualMode::start()
  {
    //Send current color
    process();
  }

  void ManualMode::process()
  {
    constexpr qsizetype ArraySize = 4;

    QByteArray bytes{ ArraySize, Qt::Initialization::Uninitialized };
    bytes[0] = 'm';
    bytes[1] = static_cast<char>(remap<int>(m_hue.value(), 0, 359, 0, 255));
    bytes[2] = static_cast<char>(m_saturation.value());
    bytes[3] = static_cast<char>(m_value.value());
    emit dataReady(bytes);
  }

  void ManualMode::save()
  {
    auto& settings = Settings::getInstance();
    settings.set(name(), SettingsKeys::Hue, m_hue.value());
    settings.set(name(), SettingsKeys::Saturation, m_saturation.value());
    settings.set(name(), SettingsKeys::Value, m_value.value());
  }

  void ManualMode::load()
  {
    const auto& settings = Settings::getInstance();
    m_hue        = settings.getInt(name(), SettingsKeys::Hue, m_hue.min(), m_hue.max(), 0);
    m_saturation = settings.getInt(name(), SettingsKeys::Saturation, m_saturation.min(), m_saturation.max(), 255);
    m_value      = settings.getInt(name(), SettingsKeys::Value, m_value.min(), m_value.max(), 255);
    onHsvChanged();
  }

  void ManualMode::setColor(const QColor &newColor)
  {
    if (m_color == newColor)
    {
      return;
    }

    m_hue = newColor.hue();
    m_saturation = newColor.saturation();
    m_value = newColor.value();
  }

  void ManualMode::setRefColor(const QColor &refColor)
  {
    if (m_refColor == refColor)
    {
      return;
    }

    m_refColor = refColor;
    emit refColorChanged(m_refColor);
  }

  void ManualMode::onHsvChanged()
  {
    m_color.setHsv(m_hue.value(), m_saturation.value(), m_value.value());
    emit colorChanged(m_color);
    process();
  }

  void ManualMode::onHueChanged()
  {
    setRefColor(QColor::fromHsv(m_hue.value(), 255, 255));
  }
};
