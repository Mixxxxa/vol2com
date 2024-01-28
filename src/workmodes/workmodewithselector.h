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

#ifndef WORKMODEWITHSELECTOR_H
#define WORKMODEWITHSELECTOR_H

#include "workmodebase.h"
#include "gui/abstractbandviewer.h"
#include "utils/boundedvalue.h"
#include <memory>
#include <functional>
#include <QColor>

class QTimer;

namespace vol2com
{
  class BassLibWrapper;
  class Equalizer;

  class WorkModeWithSelector : public WorkModeBase
  {
    Q_OBJECT
    Q_PROPERTY(vol2com::BoundedValue* uprate READ uprate CONSTANT)
    Q_PROPERTY(vol2com::BoundedValue* selectedBand READ selectedBand CONSTANT)
    Q_PROPERTY(bool showBackground READ showBackground WRITE setShowBackground NOTIFY showBackgroundChanged)
    Q_PROPERTY(vol2com::AbstractBandViewer::BarsStyle barsStyle READ barsStyle WRITE setBarsStyle NOTIFY barsStyleChanged)
    Q_PROPERTY(vol2com::AbstractBandViewer::ColorSource colorSource READ colorSource WRITE setColorSource NOTIFY colorSourceChanged)

  public:
    typedef std::function<QColor(const uint8_t&)> ColorFunc;

    enum class SettingsKeys
    {
      UpdateRate,
      SelectedBand,
      ShowBackground,
      BarsStyle,
      ColorSource
    };
    Q_ENUM(SettingsKeys)

    explicit WorkModeWithSelector(QObject *parent = nullptr);

    virtual QString name() const override =0;
    virtual QString qmlDelegate() const override =0;
    virtual QColor getGuiColor(uint8_t value) const =0;

    vol2com::BoundedValue* uprate() { return &m_uprate; }
    vol2com::BoundedValue* selectedBand() { return &m_selectedBand; }
    bool showBackground() const { return m_showBackground; }
    AbstractBandViewer::BarsStyle barsStyle() const { return m_barsStyle; }
    AbstractBandViewer::ColorSource colorSource() const { return m_colorSource; }

  public slots:
    virtual void start() override;
    virtual void stop() override;
    virtual void process() override = 0;
    virtual void save() override;
    virtual void load() override;

    void setShowBackground(bool showBackground);
    void setBarsStyle(AbstractBandViewer::BarsStyle barsStyle);
    void setColorSource(AbstractBandViewer::ColorSource colorSource);

  signals:
    void showBackgroundChanged(bool showBackground);
    void barsStyleChanged(AbstractBandViewer::BarsStyle barsStyle);
    void colorSourceChanged(AbstractBandViewer::ColorSource colorSource);

  protected:
    std::shared_ptr<BassLibWrapper> m_bassLib;
    std::shared_ptr<Equalizer> m_eq;
    QTimer* m_timer;

    BoundedValue m_uprate;
    BoundedValue m_selectedBand;
    bool m_showBackground;
    AbstractBandViewer::BarsStyle m_barsStyle;
    AbstractBandViewer::ColorSource m_colorSource;

  protected slots:
    void onUpdadeRateChanged(int value);
  };
}

Q_DECLARE_METATYPE(vol2com::WorkModeWithSelector*)

#endif // WORKMODEWITHSELECTOR_H
