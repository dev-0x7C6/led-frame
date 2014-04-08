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
#include "classes/alc-color-samples.h"
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

#include "classes/alc-led-strip-configuration.h"

ALCLedStripConfiguration config;

ALCDeviceThread::ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent)
  :QThread(parent),
    m_device(device),
    m_emitter(0),
    m_details(details),
    m_continue(true),
    m_samples(new ALCColorSamples)
{
  m_device->moveToThread(this);

  config.add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 30, true, RGB, 1.0);
  config.add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, RGB, 0.5);
  config.add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, RGB, 0.5);
  config.add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, RGB, 0.5);

  /* //LEDY JARKA
  config.add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 6, true, GRB, 2.0);
  config.add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, RGB, 1.0);
  config.add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, RGB, 1.0);
  config.add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, RGB, 1.0);
   */
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
  int framerateLimit = 100;
  double latency[2];

  counter.start();
  latency[0] = 0;
  latency[1] = 0;

  double brightness_t;
  double rgbc[3];

  quint16 ptr = 0;
  do {
    timer.start();
    if (m_emitter)
      m_emitter->state(*m_samples);
    brightness_t = brightness(true);
    rgbc[Red] = redCorrection(true);
    rgbc[Green] = greenCorrection(true);
    rgbc[Blue] = blueCorrection(true);

    if (!m_emitter) {
      msleep(100);
      continue;
    }

    QVector < int> *colors;
    ptr = 0;

    QList < ALCLedStrip *>  strips = config.list();

    for (register int ii = 0; ii < strips.count(); ++ii) {
      ALCLedStrip *strip = strips[ii];
      colors = m_samples->scaled((ALCColorSamples::Position)strip->source(), strip->count());
      const Format format = strip->colorFormat();
      const double s = strip->brightness();
      const int size = colors->size();

      if (strip->clockwise()) {
        for (register int i = 0; i < size; ++i)
          push(data, ptr, format, (*colors)[i], rgbc, brightness_t * s);
      } else {
        for (register int i = size - 1; i >= 0; --i)
          push(data, ptr, format, (*colors)[i], rgbc, brightness_t * s);
      }

    }

    m_device->write((char*)data, ptr);
    m_device->waitForBytesWritten(1000);


    latency[0] = timer.nsecsElapsed();
    latency[1] += latency[0];
    double delay = 1000000000.0/double(framerateLimit) - latency[0];
    if (delay < 0) {
      delay = 0.0;
    }

    fps++;
    usleep(delay/1000.0);

    if (counter.hasExpired(1000)) {
      counter.restart();
      latency[1] = 0;
      fps = 0;
    }

  } while (continueValue() && m_device->isWritable() && m_device->isOpen());

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

ColorEmitter *ALCDeviceThread::connectedEmitter() {
  QMutexLocker locker(m_mutex);
  return m_emitter;
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

void ALCDeviceThread::push(unsigned char *data, quint16 &ptr, Format format, quint32 color, double rgbc[], double brightness) {
  switch (format) {
  case RGB:
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    break;
  case RBG:
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    break;
  case GRB:
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    break;
  case BRG:
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    break;
  case GBR:
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    break;
  case BGR:
    data[ptr++] = max(qb(color) * rgbc[Blue] * brightness);
    data[ptr++] = max(qg(color) * rgbc[Green] * brightness);
    data[ptr++] = max(qr(color) * rgbc[Red] * brightness);
    break;
  }
}
