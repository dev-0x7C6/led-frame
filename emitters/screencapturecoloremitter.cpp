#include "screencapturecoloremitter.h"

#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QMutexLocker>
#include <QScreen>

ScreenCaptureColorEmitter::ScreenCaptureColorEmitter(QObject *parent) :
  QThread(parent),
  m_chunkSize(32),
  m_pixelSkip(8),
  m_framerateLimit(30),
  m_quit(false)
{
  moveToThread(this);
  m_screen = QGuiApplication::primaryScreen();
}

void ScreenCaptureColorEmitter::setCaptureArea(QRect capture) {
  QMutexLocker locker(&m_mutex);
  m_captureArea = capture;
}

void ScreenCaptureColorEmitter::setChunkSize(int value) {
  QMutexLocker locker(&m_mutex);
  m_chunkSize = value;
}

void ScreenCaptureColorEmitter::setPixelSkip(int value) {
  QMutexLocker locker(&m_mutex);
  m_pixelSkip = value;
}

void ScreenCaptureColorEmitter::setFramerateLimit(int value) {
  QMutexLocker locker(&m_mutex);
  m_framerateLimit = value;
}

void ScreenCaptureColorEmitter::setQuitState(bool value) {
  QMutexLocker locker(&m_mutex);
  m_quit = value;
}

#include <QDebug>
#include <QThreadPool>
#include "capturepart.h"

QMutex safeScreenCapture;

void ScreenCaptureColorEmitter::run(){
  QElapsedTimer timer;
  QElapsedTimer counter;
  QRect capture;

  int fps = 0;
  double latency[2];

  int chunkSize;
  int pixelSkip;
  int framerateLimit;
  double light;
  bool quit;

  counter.start();
  latency[0] = 0;
  latency[1] = 0;

  QList < QRgb> colors;

  do {
    if (!m_connectedCount) {
      msleep(25);
      continue;
    }

    timer.start();
    m_mutex.lock();
    capture = m_captureArea;
    chunkSize = m_chunkSize;
    pixelSkip = m_pixelSkip + 1;
    framerateLimit = m_framerateLimit;
    quit = m_quit;
    m_mutex.unlock();


    light = brightness();
    colors.clear();
    colors <<
      grab(m_screen, capture, Top, chunkSize, pixelSkip, light) <<
      grab(m_screen, capture, Right, chunkSize, pixelSkip, light) <<
      grab(m_screen, capture, Bottom, chunkSize, pixelSkip, light) <<
      grab(m_screen, capture, Left, chunkSize, pixelSkip, light);

    setState(colors);

    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double delay = 1000000000.0/double(framerateLimit) - latency[0];
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
