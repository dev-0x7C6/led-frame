#include "screen-capture-color-emitter.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QImage>
#include <qmath.h>
#include <QMutexLocker>
#include <QPixmap>
#include <QScreen>

ScreenCaptureColorEmitter::ScreenCaptureColorEmitter(QObject *parent) :
  QThread(parent),
  ColorEmitter(),
  m_chunkSize(32),
  m_pixelSkip(8),
  m_framerateLimit(30),
  m_quit(false)
{
  m_type = EMITTER_SCREEN_CAPTURE;
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


void ScreenCaptureColorEmitter::setName(QString name) {
  QMutexLocker locker(&m_mutex);
  m_name = name;
}

QRect ScreenCaptureColorEmitter::area() {
  QMutexLocker locker(&m_mutex);
  return m_captureArea;
}

int ScreenCaptureColorEmitter::chunk() {
  QMutexLocker locker(&m_mutex);
  return m_chunkSize;
}

int ScreenCaptureColorEmitter::pixelSkip() {
  QMutexLocker locker(&m_mutex);
  return m_pixelSkip;
}

int ScreenCaptureColorEmitter::framerateLimit() {
  QMutexLocker locker(&m_mutex);
  return m_framerateLimit;
}

QString ScreenCaptureColorEmitter::name() {
  QMutexLocker locker(&m_mutex);
  return m_name;
}


void ScreenCaptureColorEmitter::run(){
  QElapsedTimer timer;
  QElapsedTimer counter;
  QRect capture;

  int fps = 0;
  double latency[2];
  bool quit;

  counter.start();
  latency[0] = 0;
  latency[1] = 0;

  do {
    timer.start();
    m_mutex.lock();
    m_safeCaptureArea = m_captureArea;
    m_safeChunkSize = m_chunkSize;
    m_safePixelSkip = m_pixelSkip + 1;
    m_safeFramerateLimit = m_framerateLimit;
    m_safeBrightness = m_brightness;
    quit = m_quit;

    if (!m_connectedCount) {
      m_mutex.unlock();
      msleep(100);
      continue;
    } else
      m_mutex.unlock();

    grab(Top, m_samples);
    grab(Right, m_samples);
    grab(Bottom, m_samples);
    grab(Left, m_samples);

    setState(m_samples);

    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double delay = 1000000000.0/double(m_safeFramerateLimit) - latency[0];
    if (delay < 0) {
      delay = 0.0;
    }

    fps++;
    usleep(delay/1000.0);

    if (counter.hasExpired(1000)) {
      emit updateStats(fps, (latency[1]/double(fps)/1000000.0), latency[1]/10000000.0);
      counter.restart();
      latency[1] = 0;
      qDebug() << fps;
      fps = 0;
    }

  } while (!quit);
}

void ScreenCaptureColorEmitter::grab(ScreenFragments fragment, ColorSamples &samples) {
  quint64 r = 0;
  quint64 g = 0;
  quint64 b = 0;
  QImage image;

  double x_shift;
  double x_chunk;
  double y_shift;
  double y_chunk;

  double chunks = samples.scale();
  QVector < int> *colors;

  switch (fragment) {
  case Top:
    colors = samples.get(ColorSamples::SAMPLE_TOP);
    image = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y(),
                               m_safeCaptureArea.width(), m_safeChunkSize).toImage();
    x_chunk = image.width() / chunks;
    y_chunk = m_safeChunkSize;
    break;
  case Bottom:
    colors = samples.get(ColorSamples::SAMPLE_BOTTOM);
    image = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y() +
                               m_safeCaptureArea.height() - m_safeChunkSize, m_safeCaptureArea.width()).toImage();
    x_chunk = image.width() / chunks;
    y_chunk = m_safeChunkSize;
    break;
  case Left:
    colors = samples.get(ColorSamples::SAMPLE_LEFT);
    image = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y() + m_safeChunkSize,
                               m_safeChunkSize, m_safeCaptureArea.height() - m_safeChunkSize * 2).toImage();
    x_chunk = m_safeChunkSize;
    y_chunk = image.height() / chunks;
    break;
  case Right:
    colors = samples.get(ColorSamples::SAMPLE_RIGHT);
    image = m_screen->grabWindow(0, m_safeCaptureArea.x() + m_safeCaptureArea.width() - m_safeChunkSize,
                               m_safeCaptureArea.y() + m_safeChunkSize, m_safeChunkSize,
                               m_safeCaptureArea.height() - m_safeChunkSize * 2).toImage();
    x_chunk = m_safeChunkSize;
    y_chunk = image.height() / chunks;
    break;
  }

  for (register int i = 0; i < chunks; ++i) {
    r = g = b = 0;

    switch (fragment) {
    case Top:
      x_shift = i * (image.width() - x_chunk) / chunks;
      y_shift = 0;
      break;
    case Bottom:
      x_shift = (chunks-i) * (image.width() - x_chunk) / chunks;
      y_shift = 0;
      break;
    case Left:
      x_shift = 0;
      y_shift = (chunks-i) * (image.height() - y_chunk) / chunks;
      break;
    case Right:
      x_shift = 0;
      y_shift = i * (image.height() - y_chunk)  / chunks;
      break;
    }

    int rgb = 0;
    int counter = 0;

    for (int x = 0; x < x_chunk; x += 4) {
      for (int y = 0; y < y_chunk; y += 4) {
        rgb = image.pixel(x_shift + x, y_shift + y);
        r += uchar(rgb >> 16);
        g += uchar(rgb >> 8);
        b += uchar(rgb);
        counter++;
      }
    }

    r /= counter; r *= m_safeBrightness;
    g /= counter; g *= m_safeBrightness;
    b /= counter; b *= m_safeBrightness;

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    (*colors)[i] = qRgb(r, g, b);
  }

}

