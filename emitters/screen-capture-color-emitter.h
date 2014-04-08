#ifndef SCREENCAPTURECOLOREMITTER_H
#define SCREENCAPTURECOLOREMITTER_H



#include <QThread>
#include <QPixmap>
#include <QMutex>
#include <QList>
#include <QRect>
#include <QRgb>

class QScreen;

enum ScreenFragments {
  Bottom = 0,
  Left,
  Top,
  Right
};

#include "emitters/color-emitter.h"
#include "classes/alc-color-samples.h"

class ScreenCaptureColorEmitter : public QThread, public ColorEmitter {
  Q_OBJECT
public:
  ScreenCaptureColorEmitter(QObject *parent = 0);

private:
  ALCColorSamples m_samples;
  QScreen *m_screen;
  QString m_name;

  QRect m_captureArea;
  int m_chunkSize;
  int m_pixelSkip;
  int m_framerateLimit;
  bool m_quit;
  double m_marginProcent;

  QVector < int> *colors[4];

public slots:
  void setName(QString name);
  void setCaptureArea(QRect);
  void setChunkSize(int);
  void setPixelSkip(int);
  void setFramerateLimit(int);
  void setQuitState(bool);
  void setMarginProcent(double);

  QRect area();
  int chunk();
  int pixelSkip();
  int framerateLimit();
  double marginProcent();

  QString name();

public:
  bool configure();

protected:
  void run();

signals:
  void update(ALCColorSamples *samples);
  void updateStats(quint32 fps, double latency, double usage);
  
};

#endif // SCREENCAPTURECOLOREMITTER_H
