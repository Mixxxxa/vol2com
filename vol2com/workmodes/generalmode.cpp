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

#include "generalmode.h"
#include "basslibwrapper.h"
#include "equalizer.h"
#include "utils/math.hpp"
#include <QByteArray>

using namespace vol2com;

GeneralMode::GeneralMode(QObject *parent)
  : WorkModeWithSelector(parent)
{
}

QColor GeneralMode::getGuiColor(uint8_t value) const
{
    constexpr uint8_t MaxValue = 255;
    constexpr int PurpleHueHSV = 300;
    return QColor::fromHsv(remap<int>(value, 0, MaxValue, PurpleHueHSV, 0), 255, 255);
}

void GeneralMode::process()
{
    constexpr auto ArraySize = 2;
    const auto data = m_bassLib->band(m_selectedBand.value());

    QByteArray bytes;
    bytes.resize(ArraySize);
    bytes[0] = 'v';
    bytes[1] = static_cast<char>(m_eq->processValue(m_selectedBand.value(), data));

    emit dataReady(bytes);
}
