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

#include "fademode.h"
#include "basslibwrapper.h"
#include "equalizer.h"
#include "settings.h"
#include "utils/boundedvalue.h"
#include "utils/math.hpp"
#include <QByteArray>

namespace
{
  static constexpr auto KeyHue = "Hue";
}

using namespace vol2com;

FadeMode::FadeMode(QObject *parent)
  : WorkModeWithSelector(parent)
  , m_hue(0, 359, 0, BoundedValue::EOverflowBehavior::Clamp)
{
}

QString FadeMode::name() const
{
  return QStringLiteral("Fade");
}

QString FadeMode::qmlDelegate() const
{
  return WorkModeBase::qmlDelegatesPath() + QStringLiteral("FadeMode.qml");
}

QColor FadeMode::getGuiColor(uint8_t value) const
{
  return QColor::fromHsv(m_hue.value(), 255, value);
}

BoundedValue *FadeMode::hue()
{
  return &m_hue;
}

void FadeMode::process()
{
  constexpr auto ArraySize = 4;
  const auto data = m_bassLib->band(m_selectedBand.value());

  QByteArray bytes;
  bytes.resize(ArraySize);
  bytes[0] = 'm';
  bytes[1] = static_cast<char>(remap<int>(m_hue.value(), m_hue.min(), m_hue.max(), 0, 255));
  bytes[2] = static_cast<char>(0xFF);
  bytes[3] = static_cast<char>(m_eq->processValue(m_selectedBand.value(), data));
  emit dataReady(bytes);
}

void FadeMode::save()
{
  WorkModeWithSelector::save();
  auto& settings = Settings::getInstance();
  settings.set(name(), QString(::KeyHue), m_hue.value());
}

void FadeMode::load()
{
  WorkModeWithSelector::load();
  auto& settings = Settings::getInstance();
  m_hue = settings.getInt(name(), QString(::KeyHue), m_hue.min(), m_hue.max(), 0);
}
