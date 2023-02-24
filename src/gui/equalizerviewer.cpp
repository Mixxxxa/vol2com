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

#include "equalizerviewer.h"
#include "workmodes/workmodebase.h"
#include "controller.h"
#include "equalizer.h"
#include "utility.h"
#include <QPainter>
#include <algorithm>

using namespace vol2com;

EqualizerViewer::EqualizerViewer(QQuickItem* parent) :
    AbstractBandViewer(parent),
    m_bassLib(Controller::getInstance().bassLib()),
    m_eq(Controller::getInstance().equalizer()),
    m_data({}),
    m_eqEnabled(m_eq->enabled())
{
    QObject::connect(m_eq.get(), &Equalizer::enabledChanged, [this](bool enabled)
    {
        m_eqEnabled = enabled;
    });

    const auto& style = AppStyle::getInstance();
    applyStyles(style.theme());
    //applyStyles(AppStyle::Theme::Dark);
}

void EqualizerViewer::paint(QPainter* painter)
{
    AbstractBandViewer::paint(painter);
    const int ItemHeight = height() - BarsPadding * 2;
    const auto startY = height() - BarsPadding;
    const auto& EqData = m_eq->all();
    const bool OverAllActive = m_eq->overallAmplificationActive();

    for(size_t i = 0; i < m_data.size(); ++i)
    {
        if(m_data[i] == 0)
        {
          return;
        }

        const auto& rect = m_rects[i];
        const int HeightBase = remap<int>(m_data[i], 0, 255, 0 , ItemHeight);
        painter->fillRect(rect.x(), startY, rect.width(), -HeightBase, m_normalBrush);

        if(m_eqEnabled && (OverAllActive || EqData[i] != Equalizer::DefaultValue))
        {
            const int HeightProc = remap<int>(m_eq->processValue(static_cast<int>(i), m_data[i]), 0, 255, 0 , ItemHeight);
            const int HeightDiff = HeightProc - HeightBase;

            if(HeightDiff > 0)
            {
                painter->fillRect(rect.x(), startY - HeightBase,
                                  rect.width(), -HeightDiff,
                                  m_positiveBrush);
            }
            else if (HeightDiff < 0)
            {
                painter->fillRect(rect.x(), startY - HeightBase,
                                  rect.width(), -HeightDiff,
                                  m_negativeBrush);
            }
        }
    }
}

void EqualizerViewer::applyStyles(const AppStyle::Theme& theme)
{
    AbstractBandViewer::applyStyles(theme);

    if(theme == AppStyle::Theme::Dark)
    {
        m_normalBrush   = QColor(255, 255, 255, 128);
        m_positiveBrush = QColor(173, 255, 47, 200);
        m_negativeBrush = QColor(255, 99, 71, 200);
    }
    else
    {
        m_normalBrush = QColor(0, 0, 0, 128);
        m_positiveBrush = QColor(68, 100, 18);
        m_negativeBrush = QColor(100, 39, 28);
    }
    update();
}

void EqualizerViewer::onTimer()
{
    switch(m_bassLib->state()){
    case BassLibWrapper::State::Error:
        return;
    case BassLibWrapper::State::Idle:
        if(!m_bassLib->start())
            return;
        break;
    case BassLibWrapper::State::Active:
        break;
    }

    m_data = m_bassLib->allBands();
    update();
}
