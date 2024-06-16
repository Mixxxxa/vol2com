#include "genredetector.h"
#include "qnamespace.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QDebug>

namespace vol2com
{
  GenreDetector::GenreDetector(QObject *parent)
  : QObject{ parent }
  , m_timer{ new QTimer { this }}
  {
    using namespace std::chrono_literals;
    m_timer->setInterval(1s);
    m_timer->setTimerType(Qt::CoarseTimer);
    QObject::connect(m_timer, &QTimer::timeout,
                        this, &GenreDetector::OnTimerTick);
  }

  void GenreDetector::Start()
  {
    m_internalCounter = 0;
    SetProgress(0);
    SetDetectionInProgress(false);

    if(!m_timer->isActive())
    {
      m_timer->start();
    }
  }

  void GenreDetector::Stop()
  {
    m_timer->stop();
    SetDetectionInProgress(false);
  }

  GenreDetector::EGenre GenreDetector::CurrentGenre() const
  {
    return m_currentGenre;
  }

  int GenreDetector::Progress() const
  {
    return m_progress;
  }

  bool GenreDetector::DetectionInProgress() const
  {
    return m_detectionInProgress;
  }

  QString GenreDetector::GenreToText(EGenre genre)
  {
    using namespace Qt::Literals::StringLiterals;

    switch(genre)
    {
    case EGenre::Unknown:      return u"Unknown"_s;
    case EGenre::Electronic:   return u"Electronic"_s;
    case EGenre::Synthwave:    return u"Synthwave"_s;
    case EGenre::HipHop:       return u"Hip-Hop"_s;
    case EGenre::Metal:        return u"Metal"_s;
    case EGenre::Rock:         return u"Rock"_s;
    case EGenre::Pop:          return u"Pop"_s;
    case EGenre::Instrumental: return u"Instrumental"_s;
    case EGenre::Chill:        return u"Chill"_s;
    case EGenre::Funk:         return u"Funk"_s;
    default:                   return u"Unknown"_s;
    }
  }

  void GenreDetector::OnTimerTick()
  {
    ++m_internalCounter;

    //auto progress = Progress();
    if(m_internalCounter >= 23u)
    {
      qDebug() << "detection";
      SetDetectionInProgress(false);
      SetProgress(0);
      OnGenreDetected();
      m_internalCounter = 0;
    }
    else if(m_internalCounter >= 10u)
    {
      qDebug() << "work";
      SetDetectionInProgress(true);
      SetProgress(static_cast<int>(m_internalCounter) - 10); // 0-10
    }
    qDebug() << "INT" << m_internalCounter;
  }

  void GenreDetector::SetProgress(int value)
  {
    if(value != m_progress)
    {
      m_progress = value;
      emit ProgressChanged(m_progress);
    }
  }

  void GenreDetector::SetDetectionInProgress(bool value)
  {
    if(value != m_detectionInProgress)
    {
      m_detectionInProgress = value;
      emit DetectionInProgressChanged(m_detectionInProgress);
    }
  }

  void GenreDetector::SetGenre(EGenre genre)
  {
    if(genre != m_currentGenre)
    {
      m_currentGenre = genre;
      emit GenreChanged(m_currentGenre);
    }
  }

  void GenreDetector::OnGenreDetected()
  {
    // temporary placeholder
    int value = QRandomGenerator::global()->bounded(static_cast<int>(EGenre::EGENRES_BEGIN),
                                                    static_cast<int>(EGenre::EGENRES_END) + 1);
    if(  value >= static_cast<int>(EGenre::EGENRES_BEGIN)
      && value <= static_cast<int>(EGenre::EGENRES_END))
    {
      SetGenre(static_cast<GenreDetector::EGenre>(value));
    }
  }
}
