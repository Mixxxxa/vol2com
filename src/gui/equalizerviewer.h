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

#ifndef EQUALIZERVIEWER_H
#define EQUALIZERVIEWER_H

#include "abstractbandviewer.h"
#include "basslibwrapper.h"
#include <QBrush>
#include <QtQml/qqmlregistration.h>

namespace vol2com
{
    class Equalizer;

    class EqualizerViewer : public AbstractBandViewer
    {
        Q_OBJECT
        QML_ELEMENT

    public:
        EqualizerViewer(QQuickItem* parent = nullptr);

        virtual void paint(QPainter* painter) override;

    public slots:
        virtual void applyStyles(const AppStyle::Theme& theme) override;

    protected slots:
        virtual void onTimer() override;

    private:
        std::shared_ptr<BassLibWrapper> m_bassLib;
        std::shared_ptr<Equalizer> m_eq;
        BassLibWrapper::BassLibData m_data;
        QBrush m_normalBrush;
        QBrush m_positiveBrush;
        QBrush m_negativeBrush;
        bool m_eqEnabled;
    };
}

#endif // EQUALIZERVIEWER_H
