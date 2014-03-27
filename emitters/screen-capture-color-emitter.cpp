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

    grab(m_samples);

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

void ScreenCaptureColorEmitter::grab(ColorSamples &samples) {
  register quint64 r = 0;
  register quint64 g = 0;
  register quint64 b = 0;

  QImage image[4];
  double x_shift[4];
  double x_chunk[4];
  double y_shift[4];
  double y_chunk[4];

  double chunks = samples.scale();

  QVector < int> *colors[4];
  colors[static_cast< quint8>( ColorSamples::SAMPLE_BOTTOM)] = samples.get(ColorSamples::SAMPLE_BOTTOM);
  colors[static_cast< quint8>( ColorSamples::SAMPLE_LEFT)] = samples.get(ColorSamples::SAMPLE_LEFT);
  colors[static_cast< quint8>( ColorSamples::SAMPLE_TOP)] = samples.get(ColorSamples::SAMPLE_TOP);
  colors[static_cast< quint8>( ColorSamples::SAMPLE_RIGHT)] = samples.get(ColorSamples::SAMPLE_RIGHT);


  image[2] = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y(),
                               m_safeCaptureArea.width(), m_safeChunkSize).toImage();
  image[0] = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y() +
                               m_safeCaptureArea.height() - m_safeChunkSize, m_safeCaptureArea.width()).toImage();
  image[1] = m_screen->grabWindow(0, m_safeCaptureArea.x(), m_safeCaptureArea.y() + m_safeChunkSize,
                             m_safeChunkSize, m_safeCaptureArea.height() - m_safeChunkSize * 2).toImage();
  image[3] = m_screen->grabWindow(0, m_safeCaptureArea.x() + m_safeCaptureArea.width() - m_safeChunkSize,
                             m_safeCaptureArea.y() + m_safeChunkSize, m_safeChunkSize,
                             m_safeCaptureArea.height() - m_safeChunkSize * 2).toImage();

  y_chunk[0] = m_safeChunkSize;
  x_chunk[1] = m_safeChunkSize;
  y_chunk[2] = m_safeChunkSize;
  x_chunk[3] = m_safeChunkSize;

  x_chunk[0] = image[0].width() / chunks;
  y_chunk[1] = image[1].height() / chunks;
  x_chunk[2] = image[2].width() / chunks;
  y_chunk[3] = image[3].height() / chunks;

  for (register int ii = 0; ii < 4; ++ii) {
    for (register int i = 0; i < chunks; ++i) {
      r = g = b = 0;

      switch (ii) {
      case Top:
        x_shift[ii] = i * (image[ii].width() - x_chunk[ii]) / chunks;
        y_shift[ii] = 0;
        break;
      case Bottom:
        x_shift[ii] = (chunks-i) * (image[ii].width() - x_chunk[ii]) / chunks;
        y_shift[ii] = 0;
        break;
      case Left:
        x_shift[ii] = 0;
        y_shift[ii] = (chunks-i) * (image[ii].height() - y_chunk[ii]) / chunks;
        break;
      case Right:
        x_shift[ii] = 0;
        y_shift[ii] = i * (image[ii].height() - y_chunk[ii])  / chunks;
        break;
      }

      int rgb = 0;
      int counter = 0;

      for (int x = 0; x < x_chunk[ii]; x += 4) {
        for (int y = 0; y < y_chunk[ii]; y += 4) {
          rgb = image[ii].pixel(x_shift[ii] + x, y_shift[ii] + y);
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

      (*colors)[ii][i] = qRgb(r, g, b);
    }
  }
}

