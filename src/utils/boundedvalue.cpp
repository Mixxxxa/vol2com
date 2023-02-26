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

#include "boundedvalue.h"
#include <cassert>
#include <algorithm>

using namespace vol2com;

BoundedValue::BoundedValue(int min, int max, int value, QObject* parent) :
    QObject(parent),
    m_min(min),
    m_max(max),
    m_value(value)
{
    assert((value >= m_min) && (value <= m_max));
}

BoundedValue &BoundedValue::operator=(const int &other)
{
    setValue(other);
    return *this;
}

int BoundedValue::value() const
{
    return m_value;
}

BoundedValue& BoundedValue::operator+=(const int &other)
{
    setValue(m_value + other);
    return *this;
}

BoundedValue &BoundedValue::operator -=(const int &other)
{
    setValue(m_value - other);
    return *this;
}

int &BoundedValue::operator++()
{
    setValue(m_value + 1);
    return m_value;
}

int BoundedValue::operator++(int)
{
    auto value = m_value;
    setValue(m_value + 1);
    return value;
}

int &BoundedValue::operator--()
{
    setValue(m_value - 1);
    return m_value;
}

int BoundedValue::operator--(int)
{
    auto value = m_value;
    setValue(m_value - 1);
    return value;
}

int BoundedValue::min() const
{
    return m_min;
}

int BoundedValue::max() const
{
    return m_max;
}

void BoundedValue::setValue(int value)
{
    if (m_value == value)
        return;

    if(inBounds(value))
        m_value = value;
    else
        m_value = fitToBounds(value);

    emit valueChanged(m_value);
}
