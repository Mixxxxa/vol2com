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

#ifndef RANDOMMODE_H
#define RANDOMMODE_H

#include "workmodebase.h"
#include "boundedvalue.h"
#include <memory>

class QTimer;

namespace vol2com
{
    class SubModeBase;

    class RandomMode : public WorkModeBase
    {
        Q_OBJECT
        Q_PROPERTY(vol2com::RandomMode::SubMode subMode READ subMode WRITE setSubMode NOTIFY subModeChanged)
        Q_PROPERTY(vol2com::BoundedValue* uprate READ uprate CONSTANT)

    public:
        enum class SettingsKeys
        {
            SubMode,
            SpeedLevel
        };
        Q_ENUM(SettingsKeys)

        enum class SubMode
        {
            Type1,
            Type2
        };
        Q_ENUM(SubMode);

        explicit RandomMode(QObject *parent = nullptr);
        ~RandomMode();

        virtual QString name() const override
        {
            return QStringLiteral("Random");
        }
        virtual QString qmlDelegate() const override
        {
            return WorkModeBase::qmlDelegatesPath() + QStringLiteral("RandomMode.qml");
        }

        RandomMode::SubMode subMode() const { return m_subMode; }
        vol2com::BoundedValue* uprate() { return &m_uprate; }

    public slots:
        virtual void start() override;
        virtual void stop() override;
        virtual void process() override;
        virtual void save() override;
        virtual void load() override;
        void setSubMode(RandomMode::SubMode subMode);
        void onSpeedValueChanged(int value);

    signals:
        void subModeChanged(RandomMode::SubMode subMode);

    private:
        QTimer* m_timer;
        std::unique_ptr<SubModeBase> m_subModeInstance;
        RandomMode::SubMode m_subMode;
        BoundedValue m_uprate;
    };
}

#endif // RANDOMMODE_H
