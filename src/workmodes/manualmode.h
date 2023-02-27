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

#ifndef MANUALMODE_H
#define MANUALMODE_H

#include "workmodebase.h"
#include "utils/boundedvalue.h"
#include <QColor>

namespace vol2com
{
    class ManualMode : public WorkModeBase
    {
        Q_OBJECT
        Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
        Q_PROPERTY(QColor refColor READ refColor WRITE setRefColor NOTIFY refColorChanged)
        Q_PROPERTY(vol2com::BoundedValue* hue READ hue CONSTANT)
        Q_PROPERTY(vol2com::BoundedValue* saturation READ saturation CONSTANT)
        Q_PROPERTY(vol2com::BoundedValue* value READ value CONSTANT)

    public:
        enum class SettingsKeys
        {
            Hue,
            Saturation,
            Value
        };
        Q_ENUM(SettingsKeys)

        ManualMode();
        ~ManualMode();

        virtual QString name() const override
        {
            return QStringLiteral("Manual");
        }

        virtual QString qmlDelegate() const override
        {
            return WorkModeBase::qmlDelegatesPath() + QStringLiteral("ManualMode.qml");
        }

        vol2com::BoundedValue* hue() { return &m_hue; }
        vol2com::BoundedValue* saturation() { return &m_saturation; }
        vol2com::BoundedValue* value() { return &m_value; }
        QColor color() const { return m_color; }
        QColor refColor() const { return m_refColor; }

    public slots:
        virtual void start() override;
        virtual void stop() override { }
        virtual void process() override;
        virtual void save() override;
        virtual void load() override;

        void setColor(const QColor& newColor);
        void setRefColor(const QColor& refColor);

    signals:
        void colorChanged(const QColor& color);
        void refColorChanged(const QColor& refColor);

    private:
        vol2com::BoundedValue m_hue;
        vol2com::BoundedValue m_saturation;
        vol2com::BoundedValue m_value;
        QColor m_color;
        QColor m_refColor;

    private slots:
        void onHsvChanged();
        void onHueChanged();
    };
}

#endif // MANUALMODE_H
