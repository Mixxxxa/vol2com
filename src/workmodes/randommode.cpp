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

#include "randommode.h"
#include "settings.h"
#include "utils/math.hpp"
#include <QTimer>
#include <QColor>
#include <random>

namespace vol2com
{
  class SubModeBase
  {
  public:
    virtual ~SubModeBase() = default;
    virtual QColor nextColor() = 0;
  };

  class RandomSubModeType1 : public SubModeBase
  {
  public:
    RandomSubModeType1()
      : SubModeBase{ }
      , m_reverseTimer{ std::make_unique<QTimer>() }
      , m_rand        { std::random_device{}() }
      , m_reverse     { false }
      , m_hue         { 0, 359, 0 }
      , m_counter     { 0 }
    {
      m_reverseTimer->setTimerType(Qt::VeryCoarseTimer);
      QObject::connect(m_reverseTimer.get(), &QTimer::timeout,
                       [this]() {
                         m_reverse = !m_reverse;
                         m_reverseTimer->start(getTimerDuration());
                       });
      m_reverseTimer->start(getTimerDuration());
    }

    ~RandomSubModeType1()
    {
      if(m_reverseTimer->isActive())
      {
        m_reverseTimer->stop();
      }
    }

    virtual QColor nextColor() override
    {
      const auto color = QColor::fromHsv(m_hue.value(), 255, 255);
      const auto randomValue = std::rand();

      if(m_counter++ % 3 == 0)
      {
        if(m_reverse)
          m_hue -= (randomValue % 2);
        else
          m_hue += (randomValue % 2);
      }
      else
      {
        m_hue += ((randomValue % 3) - 1);
      }
      return color;
    }

    std::chrono::seconds getTimerDuration()
    {
      constexpr int Min = 10;
      constexpr int Max = 30;
      return std::chrono::seconds((m_rand() % (Max-Min)) + Min);
    }

  private:
    std::unique_ptr<QTimer> m_reverseTimer;
    std::minstd_rand m_rand; // Ok. There are no sence in 'high quality' random
    bool m_reverse;
    BoundedValue m_hue;
    unsigned m_counter;
  };

  RandomMode::RandomMode(QObject *parent)
    : WorkModeBase     { parent }
    , m_timer          { new QTimer(this) }
    , m_subModeInstance{ std::make_unique<RandomSubModeType1>() }
    , m_subMode        { RandomMode::SubMode::Type1 }
    , m_uprate         { 1, 120, 60}
  {
    onSpeedValueChanged(m_uprate.value());
    QObject::connect(m_timer, &QTimer::timeout,
                        this, &RandomMode::process);
    QObject::connect(&m_uprate, &BoundedValue::valueChanged,
                          this, &RandomMode::onSpeedValueChanged);
  }

  RandomMode::~RandomMode() = default;

  void RandomMode::start()
  {
    if(m_timer && !m_timer->isActive())
    {
      m_timer->start();
    }
  }

  void RandomMode::stop()
  {
    if(m_timer && m_timer->isActive())
    {
      m_timer->stop();
    }
  }

  void RandomMode::process()
  {
    if(!m_subModeInstance)
    {
      return;
    }

    constexpr qsizetype ArraySize = 4;
    const auto color = m_subModeInstance->nextColor();

    QByteArray bytes { ArraySize, Qt::Initialization::Uninitialized };
    bytes[0] = 'm';
    bytes[1] = static_cast<char>(remap<int>(color.hue(), 0, 359, 0, 255));
    bytes[2] = static_cast<char>(color.saturation());
    bytes[3] = static_cast<char>(color.value());
    emit dataReady(bytes);
  }

  void RandomMode::save()
  {
    auto& settings = Settings::getInstance();
    settings.set(name(), SettingsKeys::SubMode, static_cast<int>(m_subMode));
    settings.set(name(), SettingsKeys::SpeedLevel, m_uprate.value());
  }

  void RandomMode::load()
  {
    const auto &s = Settings::getInstance();

    const auto subMode = s.getEnum(name(), SettingsKeys::SubMode, RandomMode::SubMode::Type1);
    setSubMode(subMode);

    const auto uprate = s.getInt(name(), SettingsKeys::SpeedLevel, m_uprate.min(), m_uprate.max(), 60);
    m_uprate.setValue(uprate);
  }

  void RandomMode::setSubMode(RandomMode::SubMode subMode)
  {
    if (m_subMode == subMode)
    {
      return;
    }
    m_subMode = subMode;
    emit subModeChanged(m_subMode);
  }

  void RandomMode::onSpeedValueChanged(int value)
  {
    if(!m_timer)
    {
      return;
    }

    const bool wasActive = m_timer->isActive();
    if(wasActive)
    {
      m_timer->stop();
    }
    m_timer->setInterval(1000 / value);
    if(wasActive)
    {
      m_timer->start();
    }
  }
}
