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

#pragma once
#include "workmodewithselector.h"

namespace vol2com
{
  class GenreDetector;

  class AiMode : public WorkModeWithSelector
  {
    Q_OBJECT
    Q_PROPERTY(QString detectedGenre READ detectedGenre NOTIFY detectedGenreChanged)
    Q_PROPERTY(int detectionProgress READ detectionProgress NOTIFY detectionProgressChanged)
    Q_PROPERTY(bool detectionInProgress READ detectionInProgress NOTIFY detectionInProgressChanged)

  public:
    explicit AiMode(QObject *parent = nullptr);

    virtual QString name() const override
    {
      return QStringLiteral("AI");
    }

    virtual QString qmlDelegate() const override
    {
      return WorkModeBase::qmlDelegatesPath() + QStringLiteral("AiMode.qml");
    }

    virtual QColor getGuiColor(uint8_t value) const override;

    QString detectedGenre() const;
    int detectionProgress() const;
    bool detectionInProgress() const;

  public slots:
    void process() override;
    void start() override;
    void stop() override;

  signals:
    void detectedGenreChanged();
    void detectionProgressChanged();
    void detectionInProgressChanged();

  private:
    GenreDetector * const m_detector;

  private slots:
    void OnGenreChanged();

  //  void OnTimerTick();
  //  void setDetectionProgress(int newDetectionProgress);
  //  void setDetectedGenre(const QString &newDetectedGenre);
  };
}
