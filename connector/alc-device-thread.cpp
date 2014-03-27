/**********************************************************************************
 * Wiimotedev Project - http://code.google.com/p/wiimotedev/ -                    *
 * Copyright (C) 2013-2014  Bartłomiej Burdukiewicz                               *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#include "classes/alc-color-correction.h"
#include "classes/color-samples.h"
#include "connector/alc-device-thread.h"
#include "emitters/color-emitter.h"

#include <QElapsedTimer>

#define qb(b) qBlue(b)
#define qg(g) qGreen(g)
#define qr(r) qRed(r)

unsigned char max(int value) {
  if (value > 255)
    return 255; else
    return value;
}

ALCDeviceThread::ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent)
  :QThread(parent),
    m_device(device),
    m_emitter(0),
    m_details(details),
    m_continue(true),
    m_samples(new ColorSamples)
{
  m_device->moveToThread(this);
}

ALCDeviceThread::~ALCDeviceThread() {
  delete m_samples;
}

void ALCDeviceThread::run() {
  unsigned char data[2048];
  memset((char*)data, 0, sizeof(data));
  QElapsedTimer timer;
  QElapsedTimer counter;

  int fps = 0;
  int framerateLimit = 102;
  double latency[2];

  counter.start();
  latency[0] = 0;
  latency[1] = 0;
  int io_delay = 0;

  Format format_t;
  double brightness_t;
  double colorCorrection_t[3];

  quint16 ptr = 0;
  do {
    timer.start();
    if (m_emitter)
      m_emitter->state(*m_samples);
    brightness_t = brightness(true);
    colorCorrection_t[Red] = redCorrection(true);
    colorCorrection_t[Green] = greenCorrection(true);
    colorCorrection_t[Blue] = blueCorrection(true);
    format_t = RGB;

    if (!m_emitter) {
      msleep(100);
      continue;
    }

    QVector < int> *colors;
    ptr = 0;

    for (register int ii = 0; ii < 4; ++ii) {
      switch (ii) {
      case 0:
        colors  = m_samples->scaled(ColorSamples::SAMPLE_BOTTOM, 30);
        break;
      case 1:
        colors = m_samples->scaled(ColorSamples::SAMPLE_LEFT, 15);
        break;
      case 2:
        colors = m_samples->scaled(ColorSamples::SAMPLE_TOP, 30);
        break;
      case 3:
        colors = m_samples->scaled(ColorSamples::SAMPLE_RIGHT, 15);
        break;
      }

      for (register int i = 0; i < colors->size(); ++i) {
        const quint32 color = (*colors)[i];
        switch (format_t) {
        case RGB:
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          break;
        case RBG:
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          break;
        case GRB:
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          break;
        case BRG:
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          break;
        case GBR:
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          break;
        case BGR:
          data[ptr++] = max(qb(color) * colorCorrection_t[Blue] * brightness_t);
          data[ptr++] = max(qg(color) * colorCorrection_t[Green] * brightness_t);
          data[ptr++] = max(qr(color) * colorCorrection_t[Red] * brightness_t);
          break;
        }
      }
      delete colors;
    }

   // ptr += 120*3;


    m_device->write((char*)data, ptr);
    m_device->flush();
    m_device->waitForBytesWritten(1000);

    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double delay = 1000000000.0/double(framerateLimit) - latency[0];
    if (delay < 0) {
      delay = 0.0;
    }

    fps++;
    usleep(delay/1000.0);
    msleep(io_delay);

    if (counter.hasExpired(1000)) {
      counter.restart();
      latency[1] = 0;
      fps = 0;
    }


  } while (continueValue() && m_device->error() == QSerialPort::NoError);

  if (m_device->isOpen() && m_device->isWritable())
    m_device->close();

  delete m_device;

  if (m_emitter)
    m_emitter->done();
}

void ALCDeviceThread::connectEmitter(ColorEmitter *emitter) {
  QMutexLocker locker(m_mutex);
  if (m_emitter)
    m_emitter->done();

  if ((m_emitter = emitter))
    m_emitter->init();
}


QSerialPortInfo ALCDeviceThread::details() { return m_details; }

void ALCDeviceThread::setContinueValue(bool value) {
  QMutexLocker locker(m_mutex);
  m_continue = value;
}

bool ALCDeviceThread::continueValue() {
  QMutexLocker locker(m_mutex);
  return m_continue;
}
