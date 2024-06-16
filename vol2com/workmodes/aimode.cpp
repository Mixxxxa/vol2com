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

#include "aimode.h"
#include "basslibwrapper.h"
#include "equalizer.h"
#include "qtmetamacros.h"
#include "utils/math.hpp"
#include "workmodewithselector.h"
#include <QByteArray>
#include <QTimer>
#include "genredetector.h"
#include <QDebug>

namespace vol2com
{
  AiMode::AiMode(QObject *parent)
    : WorkModeWithSelector{ parent }
    , m_detector { new GenreDetector{ this }}
  {
    QObject::connect(m_detector, &GenreDetector::GenreChanged,
                           this, &AiMode::detectedGenreChanged);
    QObject::connect(m_detector, &GenreDetector::ProgressChanged,
                           this, &AiMode::detectionProgressChanged);
    QObject::connect(m_detector, &GenreDetector::DetectionInProgressChanged,
                           this, &AiMode::detectionInProgressChanged);
    QObject::connect(m_detector, &GenreDetector::GenreChanged,
                           this, &AiMode::OnGenreChanged);
  }

  QColor AiMode::getGuiColor(uint8_t value) const
  {
    constexpr uint8_t MaxValue = 255;
    constexpr int PurpleHueHSV = 300;
    return QColor::fromHsv(remap<int>(value, 0, MaxValue, PurpleHueHSV, 0), 255, 255);
  }

  void AiMode::process()
  {
    constexpr auto ArraySize = 2;
    const auto data = m_bassLib->band(m_selectedBand.value());

    QByteArray bytes;
    bytes.resize(ArraySize);
    bytes[0] = 'v';
    bytes[1] = static_cast<char>(m_eq->processValue(m_selectedBand.value(), data));

    emit dataReady(bytes);
  }

  void AiMode::start()
  {
    WorkModeWithSelector::start();
    m_detector->Start();
  }

  void AiMode::stop()
  {
    m_detector->Stop();
    WorkModeWithSelector::stop();
  }

  void AiMode::OnGenreChanged()
  {
    const auto genre = m_detector->CurrentGenre();
    const int maxBand = selectedBand()->max();

    int newBand = maxBand - 1;

    switch(genre)
    {
    case GenreDetector::EGenre::Electronic:
    case GenreDetector::EGenre::Synthwave:
    case GenreDetector::EGenre::Chill:
    case GenreDetector::EGenre::Funk:
      newBand = maxBand - 1;
      break;
    case GenreDetector::EGenre::HipHop:
    case GenreDetector::EGenre::Pop:
      newBand = 2;
      break;
    case GenreDetector::EGenre::Metal:
    case GenreDetector::EGenre::Rock:
    case GenreDetector::EGenre::Instrumental:
      newBand = 10;
      break;
    default: break; // suppress clang warning
    };

    selectedBand()->setValue(newBand);
  }

  QString AiMode::detectedGenre() const
  {
    return GenreDetector::GenreToText(m_detector->CurrentGenre());
  }

  int AiMode::detectionProgress() const
  {
    return m_detector->Progress();
  }

  bool AiMode::detectionInProgress() const
  {
    return m_detector->DetectionInProgress();
  }
}
