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

#ifndef ABSTRACTBANDVIEWER_H
#define ABSTRACTBANDVIEWER_H

#include <QQuickPaintedItem>
#include <QBrush>
#include <array>
#include "basslibwrapper.h"
#include "appstyle.h"

class QTimer;

namespace vol2com
{
    class AbstractBandViewer : public QQuickPaintedItem
    {
        Q_OBJECT

        Q_PROPERTY(int selectedBand READ selectedBand WRITE setSelectedBand NOTIFY selectedBandChanged)
        Q_PROPERTY(bool showBandsBackground READ showBandsBackground WRITE setShowBandsBackground NOTIFY showBandsBackgroundChanged)
        Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
        Q_PROPERTY(BarsStyle barsStyle READ barsStyle WRITE setBarsStyle NOTIFY barsStyleChanged)
        Q_PROPERTY(ColorSource colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)

    public:
        enum class BarsStyle
        {
            AllPainted,
            ActivePainted
        };
        Q_ENUM(BarsStyle)

        enum class ColorSource
        {
            Default,
            ActiveMode
        };
        Q_ENUM(ColorSource)

        AbstractBandViewer(QQuickItem* parent = nullptr);
        virtual ~AbstractBandViewer() {}
        virtual void paint(QPainter* painter) override;
        virtual void componentComplete() override;

        int selectedBand() const { return m_selectedBand; }
        bool showBandsBackground() const { return m_showBandsBackground; }
        bool active() const { return m_active; }
        BarsStyle barsStyle() const { return m_barsStyle; }
        ColorSource colorSource() const { return m_colorSource; }

    public slots:
        void setSelectedBand(int selectedBand);
        void setShowBandsBackground(bool showBandsBackground);
        void setActive(bool active);
        void setBarsStyle(BarsStyle barsStyle);
        void setColorSource(ColorSource colorSource);
        virtual void applyStyles(const AppStyle::Theme& theme);

    signals:
        void selectedBandChanged(int selectedBand);
        void showBandsBackgroundChanged(bool showBandsBackground);
        void activeChanged(bool active);
        void barsStyleChanged(BarsStyle barsStyle);
        void colorSourceChanged(ColorSource colorSource);
        void rightMouseClicked(QPoint pos);

    protected:
        static constexpr int BarsPadding = 10;
        static constexpr int TimerTimeout = 1000/40;

        virtual void mousePressEvent(QMouseEvent* event) override;

        QTimer* m_timer;
        std::array<QRect, BassLibWrapper::AvailableBands> m_rects;
        int m_selectedBand;
        bool m_showBandsBackground;
        bool m_active;
        BarsStyle m_barsStyle;
        ColorSource m_colorSource;
        QBrush m_backgroundBrush;
        QBrush m_activeBackgroundBrush;

    protected slots:
        virtual void onTimer() = 0;
        virtual void onWindowStateChanged(const Qt::WindowState &state);

    private:
        void recalcRects();
    };
}

Q_DECLARE_METATYPE(vol2com::AbstractBandViewer::BarsStyle)
Q_DECLARE_METATYPE(vol2com::AbstractBandViewer::ColorSource)

#endif // EQUALIZERVIEWER_H
