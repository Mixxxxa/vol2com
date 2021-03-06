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

#ifndef BOUNDEDVALUE_H
#define BOUNDEDVALUE_H

#include <QObject>

namespace vol2com
{
    class BoundedValue : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(int min READ min CONSTANT)
        Q_PROPERTY(int max READ max CONSTANT)
        Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)

    public:
        BoundedValue(int min, int max, int value, QObject* parent = nullptr);
        BoundedValue& operator=(const int& other);
        BoundedValue& operator+=(const int& other);
        BoundedValue& operator-=(const int& other);
        int& operator++();
        int operator++(int);
        int& operator--();
        int operator--(int);

        int min() const;
        int max() const;
        int value() const;

    public slots:
        void setValue(int value);

    signals:
        void valueChanged(int value);

    private:
        inline int fitToBounds(int value) const
        {
            if(value >= 0)
                return value % (m_max + 1);
            else
                return m_max - std::abs(value) % (m_max+1) + 1;
        }

        inline bool inBounds(int value) const
        {
            return ((value >= m_min) && (value <= m_max));
        }

        const int m_min;
        const int m_max;
        int m_value;
    };
}

#endif // BOUNDEDVALUE_H
