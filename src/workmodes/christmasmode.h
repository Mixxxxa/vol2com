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

#ifndef CHRISTMASMODE_H
#define CHRISTMASMODE_H

#include "workmodebase.h"
#include "utils/boundedvalue.h"
#include <QColor>
#include <memory>
#include <array>
#include <chrono>

class QTimer;

namespace vol2com
{
  class ChristmasSubMode
  {
  public:
    ChristmasSubMode() = default;
    virtual ~ChristmasSubMode() = default;

    virtual std::chrono::milliseconds timerDuration(int level) const = 0;
    virtual QColor nextColor() = 0;

  protected:
    static constexpr std::array<QColor, 4> DefaultColors {
      QColor(255,   0,   0),
      QColor(  0, 255,   0),
      QColor(  0,   0, 255),
      QColor(255, 170,   0)
    };
  };

  class ChristmasMode : public WorkModeBase
  {
    Q_OBJECT
    Q_PROPERTY(vol2com::ChristmasMode::SubMode subMode READ subMode WRITE setSubMode NOTIFY subModeChanged)
    Q_PROPERTY(vol2com::BoundedValue* uprate READ uprate CONSTANT)

  public:
    enum class SettingsKeys
    {
      SubMode,
      SpeedLevel
    };
    Q_ENUM(SettingsKeys)

    enum class SubMode
    {
      Fixed,
      Fading
    };
    Q_ENUM(SubMode)

    ChristmasMode();
    ~ChristmasMode();

    virtual QString name() const override
    {
      return QStringLiteral("Christmas");
    }

    virtual QString qmlDelegate() const override
    {
      return WorkModeBase::qmlDelegatesPath() + QStringLiteral("ChristmasMode.qml");
    }

    vol2com::ChristmasMode::SubMode subMode() const { return m_subMode; }
    vol2com::BoundedValue* uprate() { return &m_uprate; }

  public slots:
    virtual void start() override;
    virtual void stop() override;
    virtual void process() override;
    virtual void save() override;
    virtual void load() override;
    void setSubMode(vol2com::ChristmasMode::SubMode subMode);

  signals:
    void subModeChanged(vol2com::ChristmasMode::SubMode subMode);

  private:
    QTimer* m_timer;
    std::unique_ptr<ChristmasSubMode> m_subModeInstance;
    ChristmasMode::SubMode m_subMode;
    BoundedValue m_uprate;

  private slots:
    void onSubModeChanged(ChristmasMode::SubMode mode);
    void updateTimerDuration();
  };
}

#endif // CHRISTMASMODE_H
