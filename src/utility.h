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

#ifndef UTILITY_H
#define UTILITY_H

namespace vol2com
{
    template <typename T>
    static inline constexpr T remap(T x, T in_min, T in_max, T out_min, T out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    template <typename T>
    static inline constexpr T lerp(T startValue, T endValue, T step, T totalSteps)
    {
        return (endValue - startValue) * step / totalSteps + startValue;
    }
}

#endif // UTILITY_H
