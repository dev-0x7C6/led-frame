#include "capturethread.h"

#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QMutexLocker>

CaptureThread::CaptureThread(QObject *parent) :
  QThread(parent),
  m_alghoritm(1),
  m_chunkSize(32),
  m_pixelSkip(8),
  m_framerateLimit(30),
  m_brightness(1.0),
  m_quit(false)
{
  m_screen = QGuiApplication::primaryScreen();
}

void CaptureThread::setCaptureArea(QRect capture) {
  QMutexLocker locker(&m_mutex);
  m_captureArea = capture;
}

void CaptureThread::setAlghoritm(int value) {
  QMutexLocker locker(&m_mutex);
  m_alghoritm = value;
}

void CaptureThread::setChunkSize(int value) {
  QMutexLocker locker(&m_mutex);
  m_chunkSize = value;
}

void CaptureThread::setPixelSkip(int value) {
  QMutexLocker locker(&m_mutex);
  m_pixelSkip = value;
}

void CaptureThread::setFramerateLimit(int value) {
  QMutexLocker locker(&m_mutex);
  m_framerateLimit = value;
}

void CaptureThread::setBrightness(double value) {
  QMutexLocker locker(&m_mutex);
  m_brightness = value;
}

void CaptureThread::setQuitState(bool value) {
  QMutexLocker locker(&m_mutex);
  m_quit = value;
}

void CaptureThread::run(){
  QList < QRgb> list;
  QElapsedTimer timer;
  QElapsedTimer counter;
  QPixmap wholeScreen;
  QPixmap screenChunk[4];
  QImage imageCache;
  quint64 rgbCache[3];
  QRect capture;

  int fps = 0;
  double latency[2];

  int alghoritm;
  int chunkSize;
  int pixelSkip;
  int framerateLimit;
  double brightness;
  bool quit;

  counter.start();
  latency[0] = 0;
  latency[1] = 0;
  do {
    timer.start();
    m_mutex.lock();
    capture = m_captureArea;
    alghoritm = m_alghoritm;
    chunkSize = m_chunkSize;
    pixelSkip = m_pixelSkip + 1;
    framerateLimit = m_framerateLimit;
    brightness = m_brightness;
    quit = m_quit;
    m_mutex.unlock();

    list.clear();
    switch (alghoritm) {
    case ScreenCaptureFullShots:
      wholeScreen = m_screen->grabWindow(0, capture.x(), capture.y(), capture.width(), capture.height());
      break;
    case ScreenCapturePartialShots:
      screenChunk[0] = m_screen->grabWindow(0, capture.x(), capture.y(), capture.width(), chunkSize);
      screenChunk[1] = m_screen->grabWindow(0, capture.x(), capture.y() + capture.height() - chunkSize, capture.width(), chunkSize);
      screenChunk[2] = m_screen->grabWindow(0, capture.x(), capture.y(), chunkSize, capture.height());
      screenChunk[3] = m_screen->grabWindow(0, capture.x() + capture.width() - chunkSize, capture.y(), chunkSize, capture.height());
      break;
    }

    for (register int context = 0; context < 4; ++context) {
      for (register int i = 0; i < 8; ++i) {
        memset(reinterpret_cast< void*>(rgbCache), 0, sizeof(rgbCache));

        switch (alghoritm) {
        case ScreenCaptureFullShots:
          switch (context) {
          case 0:
            imageCache = wholeScreen.copy(i * (capture.width()/8), 0, chunkSize, chunkSize).toImage();
            break;
          case 1:
            imageCache = wholeScreen.copy(i * (capture.width()/8), capture.height()-chunkSize, chunkSize , chunkSize).toImage();
            break;
          case 2:
            imageCache = wholeScreen.copy(0, i * (capture.height()/8), chunkSize , chunkSize).toImage();
            break;
          case 3:
            imageCache = wholeScreen.copy(capture.width() - chunkSize, i * (capture.height()/8), chunkSize, chunkSize).toImage();
            break;
          }
          break;
        case ScreenCapturePartialShots:
          switch (context) {
          case 0:
          case 1:
            imageCache = screenChunk[context].copy(i * capture.width() / 8, 0, chunkSize, chunkSize).toImage();
            break;
          case 2:
          case 3:
            imageCache = screenChunk[context].copy(0, i * capture.height() / 8, chunkSize, chunkSize).toImage();
            break;
          }
          break;
        case ScreenCaptureCriticalShots:
          switch (context) {
          case 0:
            imageCache = m_screen->grabWindow(0, capture.x() + i * (capture.width()/8), capture.y(), chunkSize, chunkSize).toImage();
            break;
          case 1:
            imageCache = m_screen->grabWindow(0, capture.x() + i * (capture.width()/8), capture.y()+capture.height() - chunkSize, chunkSize, chunkSize).toImage();
            break;
          case 2:
            imageCache = m_screen->grabWindow(0, capture.x(), capture.y() + i * (capture.height()/8), chunkSize, chunkSize).toImage();
            break;
          case 3:
            imageCache = m_screen->grabWindow(0, capture.x() + capture.width() - chunkSize, capture.y() + i * (capture.height()/8), chunkSize, chunkSize).toImage();
            break;
          }
          break;
        }

        quint32 *source = reinterpret_cast< quint32*>(imageCache.bits());
        int length = imageCache.byteCount()/imageCache.depth();
        for (register int i = 0; i < length; i += pixelSkip) {
          rgbCache[0] += uchar(source[i] >> 16);
          rgbCache[1] += uchar(source[i] >> 8);
          rgbCache[2] += uchar(source[i]);
        }

        rgbCache[0] = double(rgbCache[0]) / double(length/(pixelSkip * brightness));
        rgbCache[1] = double(rgbCache[1]) / double(length/(pixelSkip * brightness));
        rgbCache[2] = double(rgbCache[2]) / double(length/(pixelSkip * brightness));

        for (register int c = 0; c < 3; ++c)
          if (rgbCache[c] > 255) rgbCache[c] = 255;

        list << qRgb(rgbCache[0], rgbCache[1], rgbCache[2]);
      }
    }
    emit updateLeds(list);

    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double  delay = 1000000000.0/double(framerateLimit) - latency[0];
    if (delay < 0) {
      delay = 0.0;
    }

    fps++;
    usleep(delay/1000.0);

    if (counter.hasExpired(1000)) {
      emit updateStats(fps, (latency[1]/double(fps)/1000000.0), latency[1]/10000000.0);
      counter.restart();
      latency[1] = 0;
      fps = 0;
    }

  } while (!quit);
}
