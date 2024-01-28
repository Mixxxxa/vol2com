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
#include <algorithm>
#include <utility>

namespace vol2com
{
  BoundedValue::BoundedValue(value_type min, value_type max, value_type value,
                             EOverflowBehavior behavior, QObject* parent)
    : QObject           { parent   }
    , m_min             { min      }
    , m_max             { max      }
    , m_value           { value    }
    , m_overflowBehavior{ behavior }
  {
    Q_ASSERT(min < max);
    Q_ASSERT(InRange(value));
  }

  BoundedValue &BoundedValue::operator=(value_type other)
  {
    setValue(other);
    return *this;
  }

  BoundedValue& BoundedValue::operator+=(value_type other)
  {
    setValue(m_value + other);
    return *this;
  }

  BoundedValue &BoundedValue::operator -=(value_type other)
  {
    setValue(m_value - other);
    return *this;
  }

  BoundedValue::value_type &BoundedValue::operator ++()
  {
    setValue(m_value + 1);
    return m_value;
  }

  BoundedValue::value_type BoundedValue::operator++(int)
  {
    auto value = m_value;
    setValue(m_value + 1);
    return value;
  }

  BoundedValue::value_type &BoundedValue::operator--()
  {
    setValue(m_value - 1);
    return m_value;
  }

  BoundedValue::value_type BoundedValue::operator--(int)
  {
    auto value = m_value;
    setValue(m_value - 1);
    return value;
  }

  BoundedValue::value_type BoundedValue::min() const
  {
    return m_min;
  }

  BoundedValue::value_type BoundedValue::max() const
  {
    return m_max;
  }

  BoundedValue::value_type BoundedValue::value() const
  {
    return m_value;
  }

  bool BoundedValue::InRange(value_type value) const
  {
    return std::cmp_greater_equal(value, min())
        && std::cmp_less_equal(value, max());
  }

  BoundedValue::EOverflowBehavior BoundedValue::OverflowBehavior() const
  {
    return m_overflowBehavior;
  }

  void BoundedValue::setValue(BoundedValue::value_type value)
  {
    if (m_value == value)
    {
      return;
    }

    const auto oldValue = m_value;

    if(m_overflowBehavior == EOverflowBehavior::Clamp)
    {
      m_value = std::clamp(value, min(), max());
    }
    else if (m_overflowBehavior == EOverflowBehavior::Overflow)
    {
      if(value >= 0)
      {
        m_value = value % (m_max + 1);
      }
      else
      {
        const auto max = this->max();
        m_value = max - std::abs(value) % (max + 1) + 1;
      }
    }

    if(m_value != oldValue)
    {
      emit valueChanged(m_value);
    }
  }
}
