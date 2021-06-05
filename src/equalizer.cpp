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

#include "equalizer.h"
#include "settings.h"

using namespace vol2com;

namespace
{
    static constexpr auto SectionEqualizer = "Equalizer";
    static constexpr auto KeyEnabled = "Enabled";
    static constexpr auto KeyOverall = "Overall";
    static constexpr auto KeyBandPlaceholder = "Band%1";

    bool checkValue(const Equalizer::EqualizerValue& input)
    {
        return (input >= Equalizer::MinimumValue) && (input <= Equalizer::MaximumValue);
    }
}

Equalizer::Equalizer(QObject *parent) :
    V2CBase(parent),
    m_overallAmplification(DefaultValue),
    m_values({}),
    m_enabled(true)
{
    setSaveTimeout(10000);
    load();
}

Equalizer::~Equalizer()
{
    save();
}

const Equalizer::EqualizerValue& Equalizer::get(int band) const
{
    return m_values[band];
}

void Equalizer::set(int band, const EqualizerValue& value)
{
    if((band < 0) || (static_cast<size_t>(band) > m_values.size())
            || (checkValue(value) == false) || (value == m_values[band]))
    {
        return;
    }

    m_values[band] = value;
    emit valueChagned(band, m_values[band]);
    scheduleSave();
}

const Equalizer::EqualizerValue& Equalizer::overallAmplification() const
{
    return m_overallAmplification;
}

void Equalizer::setOverallAmplification(const EqualizerValue& value)
{
    if (m_overallAmplification == value)
        return;

    m_overallAmplification = value;
    emit overallAmplificationChanged(m_overallAmplification);
}

bool Equalizer::overallAmplificationActive() const
{
    return m_overallAmplification != DefaultValue;
}

const Equalizer::EqualizerData& Equalizer::all() const
{
    return m_values;
}

uint8_t Equalizer::processValue(int band, uint8_t value)
{
    if(!m_enabled)
        return value;
    else if((band < 0) || (static_cast<size_t>(band) > m_values.size()))
        return static_cast<uint8_t>(0);

    auto processedValue = (value * static_cast<unsigned>(m_overallAmplification) * static_cast<unsigned>(m_values[band])) / 10000;
    if(processedValue > BassLibWrapper::MaxValue)
        return BassLibWrapper::MaxValue;
    else
        return static_cast<uint8_t>(processedValue);
}

void Equalizer::save()
{
    auto& settings = Settings::getInstance();

    settings.set(QString(::SectionEqualizer), QString(::KeyEnabled), m_enabled);
    settings.set(QString(::SectionEqualizer), QString(::KeyOverall), static_cast<int>(m_overallAmplification));
    for(size_t i = 0; i < m_values.size(); ++i)
    {
        settings.set(QString(::SectionEqualizer), QString(::KeyBandPlaceholder).arg(i), static_cast<int>(m_values[i]));
    }
}

void Equalizer::load()
{
    auto& settings = Settings::getInstance();

    m_enabled = settings.get(QString(::SectionEqualizer), QString(::KeyEnabled), true).toBool();
    m_overallAmplification = settings.getInt(QString(::SectionEqualizer),
                                             QString(::KeyOverall),
                                             MinimumValue, MaximumValue, DefaultValue);

    for(size_t i = 0; i < m_values.size(); ++i)
    {
        m_values[i] = settings.getInt(QString(::SectionEqualizer),
                                      QString(::KeyBandPlaceholder).arg(i),
                                      MinimumValue, MaximumValue, DefaultValue);
    }
}

bool Equalizer::enabled() const
{
    return m_enabled;
}

void Equalizer::setEnabled(bool enabled)
{
    if(m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}
