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

#include "abstractbandviewer.h"
#include <QPainter>
#include <QTimer>
#include "controller.h"

using namespace vol2com;

AbstractBandViewer::AbstractBandViewer(QQuickItem* parent) :
  QQuickPaintedItem(parent),
  m_timer(nullptr),
  m_selectedBand(0),
  m_showBandsBackground(true),
  m_active(false),
  m_barsStyle(BarsStyle::AllPainted),
  m_colorSource(ColorSource::Default),
  m_backgroundBrush(QColor(255, 255, 255, 16)),
  m_activeBackgroundBrush(AppStyle::getInstance().accentDark2())
{
  setFlag(ItemHasContents);
  setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
  //setRenderTarget(QQuickPaintedItem::InvertedYFramebufferObject);

  const auto& style = AppStyle::getInstance();
  applyStyles(style.theme());

  QObject::connect(this, &QQuickPaintedItem::widthChanged,
                   this, &AbstractBandViewer::recalcRects);
  QObject::connect(this, &QQuickPaintedItem::heightChanged,
                   this, &AbstractBandViewer::recalcRects);
  QObject::connect(&style, &AppStyle::themeChanged,
                   this, &AbstractBandViewer::applyStyles);
  QObject::connect(&Controller::getInstance(), &Controller::windowStateChanged,
                   this, &AbstractBandViewer::onWindowStateChanged);
}

void AbstractBandViewer::paint(QPainter* painter)
{
  const auto& SelectedItemHeight = this->height();
  const int   ItemHeight = SelectedItemHeight - BarsPadding * 2;

  if(m_showBandsBackground)
  {
    for(size_t i = 0; i < m_rects.size(); ++i)
    {
      const auto& rect = m_rects[i];
      if(static_cast<int>(i) != m_selectedBand || m_barsStyle == BarsStyle::ActivePainted)
        painter->fillRect(rect.x(), BarsPadding, rect.width(), ItemHeight, m_backgroundBrush);
    }
  }

  if(m_barsStyle == BarsStyle::AllPainted)
  {
    const auto& rect = m_rects[static_cast<size_t>(m_selectedBand)];
    painter->fillRect(rect.x(), 0, rect.width(), SelectedItemHeight, m_activeBackgroundBrush);
  }
}

void AbstractBandViewer::componentComplete()
{
  QQuickItem::componentComplete();
  recalcRects();
}

void AbstractBandViewer::setSelectedBand(int selectedBand)
{
  if (m_selectedBand == selectedBand)
    return;

  m_selectedBand = selectedBand;
  update();
  emit selectedBandChanged(m_selectedBand);
}

void AbstractBandViewer::setShowBandsBackground(bool showBandsBackground)
{
  if (m_showBandsBackground == showBandsBackground)
    return;

  m_showBandsBackground = showBandsBackground;
  update();
  emit showBandsBackgroundChanged(m_showBandsBackground);
}

void AbstractBandViewer::setActive(bool active)
{
  if (m_active == active)
    return;

  m_active = active;
  if(m_active)
  {
    if(!m_timer)
      m_timer = new QTimer(this);
    m_timer->setInterval(TimerTimeout);
    QObject::connect(m_timer, &QTimer::timeout,
                     this, &AbstractBandViewer::onTimer);
    m_timer->start();
  }
  else
  {
    if(m_timer)
    {
      m_timer->stop();
      delete m_timer;
      m_timer = nullptr;
    }
  }

  emit activeChanged(m_active);
}

void AbstractBandViewer::setBarsStyle(AbstractBandViewer::BarsStyle barsStyle)
{
  if (m_barsStyle == barsStyle)
    return;

  m_barsStyle = barsStyle;
  update();
  emit barsStyleChanged(m_barsStyle);
}

void AbstractBandViewer::setColorSource(AbstractBandViewer::ColorSource colorSource)
{
  if (m_colorSource == colorSource)
    return;

  m_colorSource = colorSource;
  update();
  emit colorSourceChanged(m_colorSource);
}

void AbstractBandViewer::applyStyles(const AppStyle::Theme& theme)
{
  const auto& style = AppStyle::getInstance();
  if(theme == AppStyle::Theme::Dark)
  {
    m_backgroundBrush = QColor(255, 255, 255, 16);
    m_activeBackgroundBrush = style.accentDark2();
  }
  else
  {
    m_backgroundBrush = QColor(0, 0, 0, 16);
    m_activeBackgroundBrush = style.accentLight1();
  }
  update();
}

void AbstractBandViewer::mousePressEvent(QMouseEvent* event)
{
  const auto button = event->button();

  if(button == Qt::LeftButton)
  {
    for(size_t i = 0; i < m_rects.size(); ++i)
    {
      if(m_rects[i].contains(event->pos()))
      {
        setSelectedBand(static_cast<int>(i));
        update();
        return;
      }
    }
  }
  else if (button == Qt::RightButton)
  {
    emit rightMouseClicked(event->pos());
  }
}

void AbstractBandViewer::onWindowStateChanged(const Qt::WindowState& state)
{
  const auto active = (state != Qt::WindowState::WindowMinimized);
  setActive(active);
}

void AbstractBandViewer::recalcRects()
{
  constexpr int Spacing = 6;
  const auto& BandsCount = BassLibWrapper::AvailableBands;
  const int bandWidth = (width() - (Spacing * (BandsCount - 1))) / BandsCount;
  const int bandHeight = static_cast<int>(this->height());

  for(int i = 0; i < BandsCount; ++i)
  {
    const int offset = (bandWidth + Spacing) * i;
    m_rects[static_cast<size_t>(i)] = QRect{ offset, 0, bandWidth, bandHeight };
  }
}
