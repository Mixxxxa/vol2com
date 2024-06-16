#pragma once

#include <QObject>
#include <QString>

class QTimer;

namespace vol2com
{
  class GenreDetector : public QObject
  {
    Q_OBJECT;

  public:
    enum class EGenre
    {
        EGENRES_BEGIN = 0
      , Unknown = EGENRES_BEGIN
      , Electronic
      , Synthwave
      , HipHop
      , Metal
      , Rock
      , Pop
      , Instrumental
      , Chill
      , Funk
      , EGENRES_END = Funk
    };
    Q_ENUM(EGenre);

    explicit GenreDetector(QObject *parent = nullptr);

    void Start();
    void Stop();

    EGenre CurrentGenre() const;
    int Progress() const;
    bool DetectionInProgress() const;

    static QString GenreToText(GenreDetector::EGenre genre);

  signals:
    void ProgressChanged(int value);
    void GenreChanged(GenreDetector::EGenre genre);
    void DetectionInProgressChanged(bool value);

  private:
    GenreDetector::EGenre m_currentGenre = EGenre::Unknown;
    int m_progress = 0;
    uint8_t m_internalCounter = 0u;
    bool m_detectionInProgress = false;
    QTimer * const m_timer;

  private slots:
    void OnTimerTick();
    void SetProgress(int value);
    void SetDetectionInProgress(bool value);
    void SetGenre(GenreDetector::EGenre genre);
    void OnGenreDetected();
  };
};
