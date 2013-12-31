#include "alcdevicethread.h"

unsigned char max(int value) {
  if (value > 255)
    return 255; else
    return value;
}

void ALCDeviceThread::run() {
  QList < QRgb> colors;

  m_mutex.lock();

  unsigned char data[1024];
  memset((char*)data, 0, sizeof(data));

  QElapsedTimer timer;
  QElapsedTimer counter;
  int fps = 0;
  int framerateLimit = 120;
  double latency[2];

  counter.start();
  latency[0] = 0;
  latency[1] = 0;

  quint16 ptr = 0;
  do {
    if (m_emitter) {
      colors = m_emitter->state();
    }

    m_mutex.unlock();

    if (colors.isEmpty()) {
      msleep(10);
      continue;
    }

    timer.start();


    ptr = 0;
    for (register int i = 0; i < colors.count()-4; ++i) {
      const quint32 color = colors[i];
      data[ptr++] = max(qg(color)*m_brightness);
      data[ptr++] = max(qr(color)*m_brightness);
      data[ptr++] = max(qb(color)*m_brightness);
    }

    m_device->write((char*)data, ptr);
    m_device->flush();

    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double delay = 1000000000.0/double(framerateLimit) - latency[0];
    if (delay < 0) {
      delay = 0.0;
    }

    fps++;
    usleep(delay/1000.0);

    if (counter.hasExpired(1000)) {
      //emit updateStats(fps, (latency[1]/double(fps)/1000000.0), latency[1]/10000000.0);
      counter.restart();
      latency[1] = 0;
      fps = 0;
    }

    m_mutex.lock();
  } while (m_continue && m_device->error() == QSerialPort::NoError);
  m_mutex.unlock();

  if (m_device->isOpen() && m_device->isWritable())
    m_device->close();

  delete m_device;

  if (m_emitter)
    m_emitter->done();
}

void ALCDeviceThread::setBrightness(double value)
{
  QMutexLocker locker(&m_mutex);
  m_brightness = value;
}
