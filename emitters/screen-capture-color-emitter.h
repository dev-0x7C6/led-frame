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
  Top = 0,
  Bottom,
  Left,
  Right
};


class QScreen;

#include "emitters/color-emitter.h"
#include "classes/color-samples.h"

class ScreenCaptureColorEmitter : public QThread, public ColorEmitter {
  Q_OBJECT
public:
  ScreenCaptureColorEmitter(QObject *parent = 0);

private:
  ColorSamples m_samples;
  QScreen *m_screen;
  QString m_name;

  QRect m_captureArea;
  int m_chunkSize;
  int m_pixelSkip;
  int m_framerateLimit;
  bool m_quit;

  QRect m_safeCaptureArea;
  int m_safeChunkSize;
  int m_safePixelSkip;
  int m_safeFramerateLimit;
  double m_safeBrightness;

private:
  void grab(ScreenFragments fragment, ColorSamples &samples);


public slots:
  void setName(QString name);
  void setCaptureArea(QRect);
  void setChunkSize(int);
  void setPixelSkip(int);
  void setFramerateLimit(int);
  void setQuitState(bool);

  QRect area();
  int chunk();
  int pixelSkip();
  int framerateLimit();

  QString name();

protected:
  void run();

signals:
  void update(ColorSamples *samples);
  void updateStats(quint32 fps, double latency, double usage);
  
};

#endif // SCREENCAPTURECOLOREMITTER_H
