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

#include "workmodewithselector.h"
#include "controller.h"
#include "settings.h"
#include <QTimer>

using namespace vol2com;

WorkModeWithSelector::WorkModeWithSelector(QObject *parent)
  : WorkModeBase    { parent }
  , m_bassLib       { Controller::getInstance().bassLib() }
  , m_eq            { Controller::getInstance().equalizer() }
  , m_timer         { new QTimer(this) }
  , m_uprate        { 1, 120, 40 }
  , m_selectedBand  { 0, BassLibWrapper::AvailableBands - 1, 0 }
  , m_showBackground{ true }
  , m_barsStyle     { AbstractBandViewer::BarsStyle::AllPainted }
  , m_colorSource   { AbstractBandViewer::ColorSource::Default }
{
  QObject::connect(&m_uprate, &BoundedValue::valueChanged,
                   this, &WorkModeWithSelector::scheduleSave);
  QObject::connect(&m_uprate, &BoundedValue::valueChanged,
                   this, &WorkModeWithSelector::onUpdadeRateChanged);
  QObject::connect(&m_selectedBand, &BoundedValue::valueChanged,
                   this, &WorkModeWithSelector::scheduleSave);
  QObject::connect(this, &WorkModeWithSelector::showBackgroundChanged,
                   this, &WorkModeWithSelector::scheduleSave);
  QObject::connect(this, &WorkModeWithSelector::barsStyleChanged,
                   this, &WorkModeWithSelector::scheduleSave);
  QObject::connect(this, &WorkModeWithSelector::colorSourceChanged,
                   this, &WorkModeWithSelector::scheduleSave);

  m_timer->setInterval(1000 / m_uprate.value());
  QObject::connect(m_timer, &QTimer::timeout,
                   this, &WorkModeWithSelector::process);
}

void WorkModeWithSelector::start()
{
  qDebug() << "void WorkModeWithSelector::start()";
  if(m_bassLib->state() != BassLibWrapper::State::Active)
  {
    m_bassLib->start();
  }

  if(m_timer && !m_timer->isActive())
  {
    m_timer->start();
  }
}

void WorkModeWithSelector::stop()
{
  qDebug() << "void WorkModeWithSelector::stop()";
  if(m_timer && m_timer->isActive())
  {
    m_timer->stop();
  }
}

void WorkModeWithSelector::save()
{
  auto& settings = Settings::getInstance();
  settings.set(name(), SettingsKeys::UpdateRate, m_uprate.value());
  settings.set(name(), SettingsKeys::SelectedBand, m_selectedBand.value());
  settings.set(name(), SettingsKeys::ShowBackground, m_showBackground);
  settings.set(name(), SettingsKeys::BarsStyle, static_cast<int>(m_barsStyle));
  settings.set(name(), SettingsKeys::ColorSource, static_cast<int>(m_colorSource));
}

void WorkModeWithSelector::load()
{
  auto& settings   = Settings::getInstance();
  m_uprate         = settings.getInt(name(), SettingsKeys::UpdateRate, m_uprate.min(), m_uprate.max(), 40);
  m_selectedBand   = settings.getInt(name(), SettingsKeys::SelectedBand, m_selectedBand.min(), m_selectedBand.max(), 0);

  const auto showBackground = settings.get(name(), SettingsKeys::ShowBackground, true).toBool();
  setShowBackground(showBackground);

  const auto barsStyle = settings.getEnum(name(), SettingsKeys::BarsStyle, AbstractBandViewer::BarsStyle::AllPainted);
  setBarsStyle(barsStyle);

  const auto colorSource = settings.getEnum(name(), SettingsKeys::ColorSource, AbstractBandViewer::ColorSource::Default);
  setColorSource(colorSource);
}

void WorkModeWithSelector::setShowBackground(bool showBackground)
{
  if (m_showBackground == showBackground)
    return;

  m_showBackground = showBackground;
  emit showBackgroundChanged(m_showBackground);
}

void WorkModeWithSelector::setBarsStyle(AbstractBandViewer::BarsStyle barsStyle)
{
  if (m_barsStyle == barsStyle)
    return;

  m_barsStyle = barsStyle;
  emit barsStyleChanged(m_barsStyle);
}

void WorkModeWithSelector::setColorSource(AbstractBandViewer::ColorSource colorSource)
{
  if (m_colorSource == colorSource)
    return;

  m_colorSource = colorSource;
  emit colorSourceChanged(m_colorSource);
}

void WorkModeWithSelector::onUpdadeRateChanged(int value)
{
  if(m_timer && m_timer->isActive())
  {
    m_timer->stop();
    m_timer->setInterval(1000 / value);
    m_timer->start();
  }
}
