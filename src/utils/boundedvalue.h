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

#pragma once

#include <QObject>

namespace vol2com
{
  /*!
   * @brief Helper class to implement bounded (min,max) value with signals
   * @todo Support corner cases (signed overflow, etc...)
   */
  class BoundedValue : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(int min READ min CONSTANT)
    Q_PROPERTY(int max READ max CONSTANT)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

  public:
    using value_type = int;

    /*!
     * @brief Overflow behavior. See each entry for details
     */
    enum class EOverflowBehavior
    {
        Overflow /*!<
                   If the value exceeds the boundary, it is safely overflows.
                   Example (0-min, 10-max): 8 + 5 -> 2
                 */
      , Clamp    /*!<
                   If the value exceeds the boundary, it becomes equal
                   to the nearest boundary.
                   Example (0-min, 10-max): 8 + 4 -> 10
                 */
    };

    BoundedValue(value_type min, value_type max, value_type value,
                 EOverflowBehavior behavior, QObject* parent = nullptr);

    BoundedValue& operator= (value_type other);
    BoundedValue& operator+=(value_type other);
    BoundedValue& operator-=(value_type other);
    value_type& operator++();
    value_type  operator++(int);
    value_type& operator--();
    value_type  operator--(int);

    value_type min() const;
    value_type max() const;
    value_type value() const;

    bool InRange(value_type value) const;
    EOverflowBehavior OverflowBehavior() const;

  public slots:
    void setValue(value_type value);

  signals:
    void valueChanged(value_type value);

  private:
    const value_type m_min;
    const value_type m_max;
    value_type m_value;
    const EOverflowBehavior m_overflowBehavior;
  };
}
