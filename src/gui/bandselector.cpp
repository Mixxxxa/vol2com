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

#include "bandselector.h"
#include "controller.h"
#include "equalizer.h"
#include "utility.h"
#include <QTimer>
#include <QPainter>

using namespace vol2com;

BandSelector::BandSelector(QQuickItem* parent) :
    AbstractBandViewer(parent),
    m_bassLib(Controller::getInstance().bassLib()),
    m_eq(Controller::getInstance().equalizer()),
    m_data({}),
    m_colorFunc(std::bind(&BandSelector::defaultColor, this, std::placeholders::_1)),
    m_activeBandBrush(Qt::blue),
    m_unactiveBandBrush(Qt::gray)
{
    QObject::connect(m_bassLib.get(), &BassLibWrapper::stateChanged,
                     this, &BandSelector::onBassLibStateChanged);
    QObject::connect(this, &BandSelector::colorSourceChanged,
                     this, &BandSelector::onColorSourceChanged);
}

void BandSelector::paint(QPainter* painter)
{
    AbstractBandViewer::paint(painter);
    const int ItemHeight = height() - BarsPadding * 2;
    const auto startY = height() - BarsPadding;
    QColor color;

    for(size_t i = 0; i < m_data.size(); ++i)
    {
        if(m_data[i] == 0)
        {
          return;
        }

        const auto& rect = m_rects[i];
        const int height = remap<int>(m_eq->processValue(static_cast<int>(i), m_data[i]), 0, 255, 0 , ItemHeight);

        if(m_barsStyle == BarsStyle::AllPainted || static_cast<int>(i) == m_selectedBand)
        {
            color = m_colorFunc(m_eq->processValue(static_cast<int>(i), m_data[i]));
            m_activeBandBrush.setColor(color);
            painter->fillRect(rect.x(), startY, rect.width(), -height, m_activeBandBrush);
        }
        else
        {
            painter->fillRect(rect.x(), startY, rect.width(), -height, m_unactiveBandBrush);
        }
    }
}

WorkModeWithSelector::ColorFunc BandSelector::colorFunc() const
{
    return m_colorFunc;
}

void BandSelector::onTimer()
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

void BandSelector::onColorSourceChanged(AbstractBandViewer::ColorSource source)
{
    if(source == ColorSource::ActiveMode)
    {
        auto mode = std::dynamic_pointer_cast<WorkModeWithSelector>(Controller::getInstance().mode());
        assert(mode);
        if(mode)
        {
            m_colorFunc = std::bind(&WorkModeWithSelector::getGuiColor, mode.get(), std::placeholders::_1);
        }
    }
    else
    {
        m_colorFunc = std::bind(&BandSelector::defaultColor, this, std::placeholders::_1);
    }
    update();
}

void BandSelector::onBassLibStateChanged(const BassLibWrapper::State& state)
{
    if(state == BassLibWrapper::State::Error)
    {
        m_data = { 0 };
        update();
    }
}

QColor BandSelector::defaultColor(const uint8_t& value) const
{
    return QColor::fromHsv(remap<short>(value, 0, 255, 120, 0), 255, 255);
}
