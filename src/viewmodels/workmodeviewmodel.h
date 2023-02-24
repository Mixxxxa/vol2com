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

#ifndef WORKMODEVIEWMODEL_H
#define WORKMODEVIEWMODEL_H

#include <QObject>
#include "basslibwrapper.h"
#include <QtQml/qqmlregistration.h>

namespace vol2com
{
    class WorkModesModel;
    class BasicQMLModel;
    class BassLibWrapper;
    class WorkModeBase;

    class WorkModeViewModel : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(vol2com::WorkModesModel* modes READ modes CONSTANT)
        Q_PROPERTY(vol2com::BasicQMLModel* devices READ devices CONSTANT)
        Q_PROPERTY(vol2com::WorkModeBase* currentMode READ currentMode NOTIFY currentModeChanged)
        Q_PROPERTY(int currentDevice READ currentDevice WRITE setCurrentDevice NOTIFY currentDeviceChanged)
        Q_PROPERTY(bool deviceError READ deviceError NOTIFY deviceErrorChanged)
        Q_PROPERTY(QString deviceErrorText READ deviceErrorText NOTIFY deviceErrorTextChanged)

    public:
        WorkModeViewModel(QObject* parent = nullptr);

        vol2com::WorkModesModel* modes() const;
        vol2com::BasicQMLModel* devices() const;
        vol2com::WorkModeBase* currentMode() const;
        int currentDevice() const;
        bool deviceError() const;
        QString deviceErrorText() const;

    public slots:
        void setCurrentDevice(int currentDevice);
        void setMode(std::shared_ptr<WorkModeBase> mode);
        void onBassStateChanged(const BassLibWrapper::State& state);

    signals:
        void currentDeviceChanged(int currentDevice);
        void currentModeChanged(vol2com::WorkModeBase* mode);
        void deviceErrorChanged(bool deviceError);
        void deviceErrorTextChanged(QString deviceErrorText);
        Q_INVOKABLE void selectMode(const QString& modeName);
        Q_INVOKABLE void clearMode();

    private:
        void setDeviceError(bool deviceError);

        std::shared_ptr<BassLibWrapper> m_bassLib;
        std::shared_ptr<WorkModesModel> m_modesModel;
        std::shared_ptr<WorkModeBase> m_currentMode;
        int m_currentDevice;
        bool m_deviceError;
        QString m_deviceErrorText;
    };
}

#endif // WORKMODEVIEWMODEL_H
