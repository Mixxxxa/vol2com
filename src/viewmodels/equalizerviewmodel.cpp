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

#include "equalizerviewmodel.h"
#include "equalizer.h"
#include "controller.h"

using namespace vol2com;

EqualizerViewModel::EqualizerViewModel(QObject* parent) :
    QObject(parent),
    m_eq(Controller::getInstance().equalizer()),
    m_enabled(m_eq->enabled()),
    m_selectedBand(0),
    m_selectedBandValue(m_eq->get(m_selectedBand)),
    m_overallValue(m_eq->overallAmplification())
{
    QObject::connect(this, &EqualizerViewModel::enabledChanged,
                     m_eq.get(), &Equalizer::setEnabled);
    QObject::connect(this, &EqualizerViewModel::overallValueChanged,
                     m_eq.get(), &Equalizer::setOverallAmplification);

    QObject::connect(m_eq.get(), &Equalizer::valueChagned,
                     this, &EqualizerViewModel::onEqualizerValueChanged);
    QObject::connect(m_eq.get(), &Equalizer::enabledChanged,
                     this, &EqualizerViewModel::setEnabled);
    QObject::connect(m_eq.get(), &Equalizer::overallAmplificationChanged,
                     this, &EqualizerViewModel::setOverallValue);
}

bool EqualizerViewModel::enabled() const
{
    return m_enabled;
}

int EqualizerViewModel::selectedBand() const
{
    return m_selectedBand;
}

Equalizer::EqualizerValue EqualizerViewModel::selectedBandValue() const
{
    return m_selectedBandValue;
}

Equalizer::EqualizerValue EqualizerViewModel::overallValue() const
{
    return m_overallValue;
}

Equalizer::EqualizerValue EqualizerViewModel::defaultValue() const
{
    return m_eq->DefaultValue;
}

Equalizer::EqualizerValue EqualizerViewModel::minValue() const
{
    return m_eq->MinimumValue;
}

Equalizer::EqualizerValue EqualizerViewModel::maxValue() const
{
    return m_eq->MaximumValue;
}

void EqualizerViewModel::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

void EqualizerViewModel::setSelectedBand(int selectedBand)
{
    if (m_selectedBand == selectedBand)
        return;

    m_selectedBand = selectedBand;
    setSelectedBandValue(m_eq->get(m_selectedBand));
    emit selectedBandChanged(m_selectedBand);
}

void EqualizerViewModel::setSelectedBandValue(Equalizer::EqualizerValue selectedBandValue)
{
    if (m_selectedBandValue == selectedBandValue)
        return;

    m_selectedBandValue = selectedBandValue;
    m_eq->set(m_selectedBand, m_selectedBandValue);
    emit selectedBandValueChanged(m_selectedBandValue);
}

void EqualizerViewModel::onEqualizerValueChanged(int band, const Equalizer::EqualizerValue& value)
{
    if(band == m_selectedBand)
    {
        setSelectedBandValue(value);
    }
}

void EqualizerViewModel::setOverallValue(Equalizer::EqualizerValue overallValue)
{
    if (m_overallValue == overallValue)
        return;

    m_overallValue = overallValue;
    emit overallValueChanged(m_overallValue);
}
