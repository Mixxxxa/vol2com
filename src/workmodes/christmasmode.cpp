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

#include "christmasmode.h"
#include "settings.h"
#include "utility.h"
#include <QTimer>

using namespace::vol2com;

namespace
{
    class FixedSubMode : public ChristmasSubMode
    {
    public:
        FixedSubMode() :
            ChristmasSubMode(),
            m_index(0)
        { }

        virtual std::chrono::milliseconds timerDuration(int level) const override
        {
            if((level < 1) || (level > 10))
                return std::chrono::milliseconds { 5000 };
            return std::chrono::milliseconds { 11000 - level * 1000 };
        }

        virtual QColor nextColor() override
        {
            const auto color = DefaultColors[m_index];
            if((++m_index) >= DefaultColors.size())
                m_index = 0;

            return color;
        }

    private:
        size_t m_index;
    };

    class FadingSubMode : public ChristmasSubMode
    {
    public:
        FadingSubMode() :
            ChristmasSubMode(),
            m_index(0),
            m_step(0),
            m_direction(FadeDirection::Increase)
        { }

        virtual std::chrono::milliseconds timerDuration(int level) const override
        {
            if((level < 1) || (level > 10))
                return std::chrono::milliseconds { 5000 };
            return std::chrono::milliseconds { 170 - level * 15 };
        }

        virtual QColor nextColor() override
        {
            constexpr int ValMin = 0;
            constexpr int ValMax = 255;

            const auto& color = DefaultColors[m_index];
            int value = lerp(ValMin, ValMax, m_step, TotalSteps);

            if(m_direction == FadeDirection::Increase)
            {
                if(++m_step > TotalSteps)
                {
                    m_step = TotalSteps;
                    m_direction = FadeDirection::Decrease;
                }
            }
            else
            {
                if(--m_step < 0)
                {
                    m_step = 0;
                    m_direction = FadeDirection::Increase;

                    if((++m_index) >= DefaultColors.size())
                        m_index = 0;
                }
            }
            return QColor::fromHsv(color.hue(), color.saturation(), value);
        }

    private:
        enum class FadeDirection
        {
            Increase,
            Decrease
        };

        static constexpr int TotalSteps = 120;
        size_t m_index;
        int m_step;
        FadeDirection m_direction;
    };
}

ChristmasMode::ChristmasMode() :
    WorkModeBase(),
    m_timer(new QTimer(this)),
    m_subModeInstance(nullptr),
    m_subMode(ChristmasMode::SubMode::Fading),
    m_uprate(1, 10, 5)
{
    QObject::connect(this, &ChristmasMode::subModeChanged,
                     this, &ChristmasMode::scheduleSave);
    QObject::connect(this, &ChristmasMode::subModeChanged,
                     this, &ChristmasMode::onSubModeChanged);
    QObject::connect(&m_uprate, &BoundedValue::valueChanged,
                     this, &ChristmasMode::scheduleSave);
    QObject::connect(&m_uprate, &BoundedValue::valueChanged,
                     this, &ChristmasMode::updateTimerDuration);
    QObject::connect(m_timer, &QTimer::timeout,
                     this, &ChristmasMode::process);
    load();
    onSubModeChanged(subMode());
}

ChristmasMode::~ChristmasMode()
{
    save();
}

void ChristmasMode::start()
{
    updateTimerDuration();
    if(m_timer && !m_timer->isActive())
        m_timer->start();
}

void ChristmasMode::stop()
{
    if(m_timer && m_timer->isActive())
        m_timer->stop();
}

void ChristmasMode::process()
{
    if(!m_subModeInstance)
        return;

    constexpr auto ArraySize = 4;
    const auto& color { m_subModeInstance->nextColor() };

    QByteArray bytes;
    bytes.resize(ArraySize);
    bytes[0] = 'm';
    bytes[1] = static_cast<char>(remap<int>(color.hue(), 0, 359, 0, 255));
    bytes[2] = static_cast<char>(color.saturation());
    bytes[3] = static_cast<char>(color.value());
    emit dataReady(bytes);
}

void ChristmasMode::save()
{
    auto& settings   = Settings::getInstance();
    settings.set(name(), SettingsKeys::SubMode, static_cast<int>(m_subMode));
    settings.set(name(), SettingsKeys::SpeedLevel, m_uprate.value());
}

void ChristmasMode::load()
{
    auto& settings   = Settings::getInstance();
    m_subMode = settings.getEnum(name(), SettingsKeys::SubMode, SubMode::Fading);
    m_uprate.setValue(settings.getInt(name(), SettingsKeys::SpeedLevel, m_uprate.min(), m_uprate.max(), 5));
}

void ChristmasMode::setSubMode(ChristmasMode::SubMode subMode)
{
    if (m_subMode == subMode)
        return;

    m_subMode = subMode;
    emit subModeChanged(m_subMode);
}

void ChristmasMode::onSubModeChanged(ChristmasMode::SubMode mode)
{
    const bool wasActive = m_timer && m_timer->isActive();

    if(wasActive)
        m_timer->stop();

    if(m_subModeInstance)
        m_subModeInstance = nullptr;

    switch (mode)
    {
    case ChristmasMode::SubMode::Fixed:
        m_subModeInstance = std::make_unique<::FixedSubMode>();
        break;
    default:
        m_subModeInstance = std::make_unique<::FadingSubMode>();
        break;
    }
    m_timer->setInterval(m_subModeInstance->timerDuration(m_uprate.value()));

    if(wasActive)
    {
        process();
        m_timer->start();
    }
}

void ChristmasMode::updateTimerDuration()
{
    if(m_timer)
    {
        const bool wasActive = m_timer->isActive();

        if(wasActive)
            m_timer->stop();

        if(m_subModeInstance)
            m_timer->setInterval(m_subModeInstance->timerDuration(m_uprate.value()));

        if(wasActive)
            m_timer->start();
    }
}
