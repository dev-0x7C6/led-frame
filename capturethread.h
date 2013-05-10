#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <QPixmap>
#include <QMutex>

class QScreen;

class CaptureThread : public QThread
{
  Q_OBJECT
private:
  QScreen *m_screen;
  QMutex m_mutex;
  QRect m_captureArea;

  int m_alghoritm;
  int m_chunkSize;
  int m_pixelSkip;
  int m_framerateLimit;
  double m_brightness;
  bool m_quit;

public:
  explicit CaptureThread(QObject *parent = 0);
  
  enum {
    ScreenFragmentsTop = 0,
    ScreenFragmentsButtom,
    ScreenFragmentsLeft,
    ScreenFragmentsRight
  };

  enum ScreenCaptureAlghoritm {
    ScreenCaptureFullShots = 0,
    ScreenCapturePartialShots,
    ScreenCaptureCriticalShots
  };

public slots:
  void setCaptureArea(QRect);
  void setAlghoritm(int);
  void setChunkSize(int);
  void setPixelSkip(int);
  void setFramerateLimit(int);
  void setBrightness(double);
  void setQuitState(bool);

protected:
  void run();

signals:
  void updateLeds(QList< QRgb> colors);
  void updateStats(quint32 fps, double latency, double usage);

};

#endif // CAPTURETHREAD_H
