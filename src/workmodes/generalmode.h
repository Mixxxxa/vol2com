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

#ifndef GENERALMODE_H
#define GENERALMODE_H

#include "workmodewithselector.h"

namespace vol2com
{
    class GeneralMode : public WorkModeWithSelector
    {
        Q_OBJECT

    public:
        explicit GeneralMode();
        ~GeneralMode();

        virtual QString name() const override
        {
            return QStringLiteral("General");
        }

        virtual QString qmlDelegate() const override
        {
            return WorkModeBase::qmlDelegatesPath() + QStringLiteral("GeneralMode.qml");
        }

        virtual QColor getGuiColor(uint8_t value) const override;

    public slots:
        virtual void process() override;
    };
}

#endif // GENERALMODE_H
