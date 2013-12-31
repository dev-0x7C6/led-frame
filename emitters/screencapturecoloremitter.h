#ifndef SCREENCAPTURECOLOREMITTER_H
#define SCREENCAPTURECOLOREMITTER_H


#include <QThread>
#include <QPixmap>
#include <QMutex>

class QScreen;

#include "emitters/coloremitter.h"

class ScreenCaptureColorEmitter : public QThread, public ColorEmitter
{
  Q_OBJECT
public:
  ScreenCaptureColorEmitter(QObject *parent = 0);

private:
  QScreen *m_screen;
  QRect m_captureArea;

  int m_chunkSize;
  int m_pixelSkip;
  int m_framerateLimit;
  bool m_quit;

public slots:
  void setCaptureArea(QRect);
  void setChunkSize(int);
  void setPixelSkip(int);
  void setFramerateLimit(int);
  void setQuitState(bool);

protected:
  void run();

signals:
  void updateLeds(QList< QRgb> colors);
  void updateStats(quint32 fps, double latency, double usage);
  
};

#endif // SCREENCAPTURECOLOREMITTER_H
