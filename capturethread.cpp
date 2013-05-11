#include "capturethread.h"

#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QMutexLocker>

CaptureThread::CaptureThread(QObject *parent) :
  QThread(parent),
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

#include <QDebug>
#include <QThreadPool>
#include "capturepart.h"

void CaptureThread::run(){
  QElapsedTimer timer;
  QElapsedTimer counter;
  QRect capture;

  int fps = 0;
  double latency[2];

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
    chunkSize = m_chunkSize;
    pixelSkip = m_pixelSkip + 1;
    framerateLimit = m_framerateLimit;
    brightness = m_brightness;
    quit = m_quit;
    m_mutex.unlock();


    emit updateLeds(QList <QRgb>() <<
      grab(m_screen, capture, Top, chunkSize, pixelSkip, brightness) <<
      grab(m_screen, capture, Bottom, chunkSize, pixelSkip, brightness) <<
      grab(m_screen, capture, Left, chunkSize, pixelSkip, brightness) <<
      grab(m_screen, capture, Right, chunkSize, pixelSkip, brightness));

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
