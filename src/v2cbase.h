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

#ifndef V2CBASE_H
#define V2CBASE_H

#include <QObject>

class QTimer;

namespace vol2com
{
    class V2CBase : public QObject
    {
        Q_OBJECT

    public:
        explicit V2CBase(QObject *parent = nullptr);
        virtual ~V2CBase() = default;

        unsigned saveTimeout() const;
        void setSaveTimeout(const unsigned& timeout_msec);

    public slots:
        void scheduleSave() const;
        virtual void save() = 0;
        virtual void load() = 0;

    private:
        QTimer* m_saveTimer;
        unsigned m_saveTimeout;
    };
}

#endif // V2CBASE_H
