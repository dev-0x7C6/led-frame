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
  m_marginProcent(0),
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

void ScreenCaptureColorEmitter::setMarginProcent(double value) {
  QMutexLocker locker(&m_mutex);
  m_marginProcent = value;
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

double ScreenCaptureColorEmitter::marginProcent() {
  QMutexLocker locker(&m_mutex);
  return m_marginProcent;
}

QString ScreenCaptureColorEmitter::name() {
  QMutexLocker locker(&m_mutex);
  return m_name;
}

#include "X11/Xutil.h"
#include "X11/Xlib.h"

void ScreenCaptureColorEmitter::run(){
  Display *display = XOpenDisplay(NULL);
  Window root = DefaultRootWindow(display);

  colors[static_cast< quint8>( ALCColorSamples::SAMPLE_BOTTOM)] = m_samples.get(ALCColorSamples::SAMPLE_BOTTOM);
  colors[static_cast< quint8>( ALCColorSamples::SAMPLE_LEFT)] = m_samples.get(ALCColorSamples::SAMPLE_LEFT);
  colors[static_cast< quint8>( ALCColorSamples::SAMPLE_TOP)] = m_samples.get(ALCColorSamples::SAMPLE_TOP);
  colors[static_cast< quint8>( ALCColorSamples::SAMPLE_RIGHT)] = m_samples.get(ALCColorSamples::SAMPLE_RIGHT);

  QElapsedTimer timer;
  QElapsedTimer counter;

  int fps = 0;
  bool quit;

  counter.start();

  double latency_curr = 0x00;
  double latency_accu = 0x00;
  double framerate_delay = 0x00;

  QRect l_captureArea;
  int l_chunkSize;
  int l_pixelSkip;
  int l_framerateLimit;
  double l_brightness;
  double l_marginProcent;

  XImage *image[4];
  XImage *src;
  int cx = 0, sx = 0;
  int cy = 0, sy = 0;
  int c = 0x00;
  quint64 r = 0x00;
  quint64 g = 0x00;
  quint64 b = 0x00;
  int width = 0x00;
  int height = 0x00;
  int chunks = m_samples.scale();
  int clipx = 0x00;
  int clipy = 0x00;


  do {
    timer.start();
    m_mutex.lock();
    l_captureArea = m_captureArea;
    l_chunkSize = m_chunkSize;
    l_pixelSkip = m_pixelSkip + 1;
    l_framerateLimit = m_framerateLimit;
    l_brightness = m_brightness;
    l_marginProcent = m_marginProcent;
    quit = m_quit;

    if (!m_connectedCount) {
      m_mutex.unlock();
      msleep(100);
      continue;
    } else
      m_mutex.unlock();

    clipx = l_captureArea.width() * l_marginProcent;
    clipy = l_captureArea.height() * l_marginProcent;

    l_captureArea.setX(l_captureArea.x() + clipx);
    l_captureArea.setY(l_captureArea.y() + clipy);
    l_captureArea.setWidth(l_captureArea.width() - clipx);
    l_captureArea.setHeight(l_captureArea.height() - clipy);

    image[2] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y(),
                         l_captureArea.width(), l_chunkSize, AllPlanes, ZPixmap);

    image[0] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y() +
                         l_captureArea.height() - l_chunkSize, l_captureArea.width(), l_chunkSize, AllPlanes, ZPixmap);

    image[1] = XGetImage(display, root, l_captureArea.x(), l_captureArea.y() + l_chunkSize,
                         l_chunkSize, l_captureArea.height() - l_chunkSize * 2, AllPlanes, ZPixmap);

    image[3] = XGetImage(display, root, l_captureArea.x() + l_captureArea.width() - l_chunkSize,
                         l_captureArea.y() + l_chunkSize, l_chunkSize,
                         l_captureArea.height() - l_chunkSize * 2, AllPlanes, ZPixmap);

    for (int ii = 0; ii < 4; ++ii) {
      src = image[ii];
      if (!src)
        continue;
      width = src->width;
      height = src->height;
      char *data = src->data;
      for (int i = 0; i < chunks; ++i) {
        switch (ii) {
        case Top:
          cx = width / chunks;
          cy = l_chunkSize;
          sx = i * (width - cx) / chunks;
          sy = 0;
          break;
        case Bottom:
          cx = image[ii]->width / chunks;
          cy = l_chunkSize;
          sx = (chunks-i) * (width - cx) / chunks;
          sy = 0;
          break;
        case Left:
          cx = l_chunkSize;
          cy = height / chunks;
          sx = 0;
          sy = (chunks-i) * (height - cy) / chunks;
          break;
        case Right:
          cx = l_chunkSize;
          cy = height / chunks;
          sx = 0;
          sy = i * (height - cy)  / chunks;
          break;
        }


        c = 0;
        register int rgb;
        for (register int x = 0; x < cx; x += 10) for (register int y = 0; y < cy; y += 10) {
          rgb = XGetPixel(src, sx + x, sy + y);
          r += (rgb >> 0x10) & 0xFF;
          g += (rgb >> 0x08) & 0xFF;
          b += (rgb) & 0xFF;
          c++;
        }

        r /= c; r *= l_brightness;
        g /= c; g *= l_brightness;
        b /= c; b *= l_brightness;

        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;

        (*colors)[ii][i] = qRgb(r, g, b);

      }
      XDestroyImage(image[ii]);
    }


    setState(m_samples);

    latency_curr = timer.nsecsElapsed();
    latency_accu += latency_curr;
    framerate_delay = 1000000000.0/static_cast< double>(l_framerateLimit) - latency_curr;
    if (framerate_delay < 0)
      framerate_delay = 0.0;

    fps++;
    usleep(framerate_delay/1000.0);

    if (counter.hasExpired(1000)) {
      emit updateStats(fps, (latency_accu/double(fps)/1000000.0), latency_accu/10000000.0);
      counter.restart();
      latency_accu = 0;
      fps = 0;
    }

  } while (!quit);
  XCloseDisplay(display);

}


