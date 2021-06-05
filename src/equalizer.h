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

#ifndef EQUALIZER_H
#define EQUALIZER_H

#include "v2cbase.h"
#include "basslibwrapper.h"
#include <array>

namespace vol2com
{
    class Equalizer : public V2CBase
    {
        Q_OBJECT

    public:
        typedef unsigned int EqualizerValue;
        typedef std::array<EqualizerValue, BassLibWrapper::AvailableBands> EqualizerData;

        static constexpr EqualizerValue MinimumValue = 50;
        static constexpr EqualizerValue MaximumValue = 200;
        static constexpr EqualizerValue DefaultValue = 100;

        explicit Equalizer(QObject *parent = nullptr);
        ~Equalizer();

        bool enabled() const;
        const EqualizerValue& overallAmplification() const;
        const EqualizerValue& get(int band) const;
        bool overallAmplificationActive() const;

        const EqualizerData& all() const;
        uint8_t processValue(int band, uint8_t value);
        void set(int band, const EqualizerValue& value);

    public slots:
        virtual void save() override;
        virtual void load() override;

        void setEnabled(bool enabled);
        void setOverallAmplification(const EqualizerValue& value);

    signals:
        void valueChagned(int band, const EqualizerValue& value);
        void overallAmplificationChanged(const EqualizerValue& value);
        void enabledChanged(bool enabled);

    private:
        EqualizerValue m_overallAmplification;
        EqualizerData m_values;
        bool m_enabled;
    };
}

Q_DECLARE_METATYPE(vol2com::Equalizer::EqualizerValue)

#endif // EQUALIZER_H
