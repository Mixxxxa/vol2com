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

#ifndef EQUALIZERVIEWMODEL_H
#define EQUALIZERVIEWMODEL_H

#include <QObject>
#include "equalizer.h"

namespace vol2com
{
    class EqualizerViewModel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(vol2com::Equalizer::EqualizerValue defaultValue READ defaultValue CONSTANT)
        Q_PROPERTY(vol2com::Equalizer::EqualizerValue minValue READ minValue CONSTANT)
        Q_PROPERTY(vol2com::Equalizer::EqualizerValue maxValue READ maxValue CONSTANT)
        Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
        Q_PROPERTY(int selectedBand READ selectedBand WRITE setSelectedBand NOTIFY selectedBandChanged)
        Q_PROPERTY(vol2com::Equalizer::EqualizerValue selectedBandValue READ selectedBandValue WRITE setSelectedBandValue NOTIFY selectedBandValueChanged)
        Q_PROPERTY(vol2com::Equalizer::EqualizerValue overallValue READ overallValue WRITE setOverallValue NOTIFY overallValueChanged)

    public:
        EqualizerViewModel(QObject* parent = nullptr);

        bool enabled() const;
        int selectedBand() const;
        vol2com::Equalizer::EqualizerValue selectedBandValue() const;
        vol2com::Equalizer::EqualizerValue overallValue() const;
        vol2com::Equalizer::EqualizerValue defaultValue() const;
        vol2com::Equalizer::EqualizerValue minValue() const;
        vol2com::Equalizer::EqualizerValue maxValue() const;

    public slots:
        void setEnabled(bool enabled);
        void setSelectedBand(int selectedBand);
        void setSelectedBandValue(vol2com::Equalizer::EqualizerValue selectedBandValue);
        void onEqualizerValueChanged(int band, const Equalizer::EqualizerValue& value);
        void setOverallValue(vol2com::Equalizer::EqualizerValue overallValue);

    signals:
        void enabledChanged(bool enabled);
        void selectedBandChanged(int selectedBand);
        void selectedBandValueChanged(vol2com::Equalizer::EqualizerValue selectedBandValue);
        void overallValueChanged(vol2com::Equalizer::EqualizerValue overallValue);

    private:
        std::shared_ptr<Equalizer> m_eq;
        bool m_enabled;
        int m_selectedBand;
        vol2com::Equalizer::EqualizerValue m_selectedBandValue;
        vol2com::Equalizer::EqualizerValue m_overallValue;
    };
}

#endif // EQUALIZERVIEWMODEL_H
