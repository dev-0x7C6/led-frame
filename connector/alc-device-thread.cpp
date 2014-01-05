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

#include "connector/alc-device-thread.h"
#include "emitters/coloremitter.h"

#include <QElapsedTimer>

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
    m_continue(true)
{
  m_device->moveToThread(this);
  for (register int i = 0; i < 3; ++i)
    m_colorCorrection[i] = 1.0;
  m_brightness = 1.0;
  m_delay = 0;
  m_format = GRB;
}

void ALCDeviceThread::run() {
  QList < QRgb> colors;

  m_mutex.lock();
  unsigned char data[1024];
  memset((char*)data, 0, sizeof(data));

  QElapsedTimer timer;
  QElapsedTimer counter;
  int fps = 0;
  int framerateLimit = 140;
  double latency[2];

  counter.start();
  latency[0] = 0;
  latency[1] = 0;
  int io_delay = 0;
  ColorFormat format;
  double brightness;
  double colorCorrection[3];

  quint16 ptr = 0;
  do {
    timer.start();
    if (m_emitter) {
      colors = m_emitter->state();
    }
    brightness = m_brightness;
    colorCorrection[Red] = m_colorCorrection[Red];
    colorCorrection[Green] = m_colorCorrection[Green];
    colorCorrection[Blue] = m_colorCorrection[Blue];
    io_delay = m_delay;
    format = m_format;
    m_mutex.unlock();

    if (colors.isEmpty()) {
      msleep(10);
      continue;
    }

    ptr = 0;
    for (register int i = 0; i < (colors.count()-4)/2; ++i) {
      const quint32 color = colors[i];
      for (register int j = 0; j < 2; ++j)
      switch (m_format) {
      case RGB:
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        break;
      case RBG:
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        break;
      case GRB:
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        break;
      case BRG:
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        break;
      case GBR:
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        break;
      case BGR:
        data[ptr++] = max(qb(color) * colorCorrection[Blue] * brightness);
        data[ptr++] = max(qg(color) * colorCorrection[Green] * brightness);
        data[ptr++] = max(qr(color) * colorCorrection[Red] * brightness);
        break;
      }
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
    msleep(io_delay);

    if (counter.hasExpired(1000)) {
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

void ALCDeviceThread::connectEmitter(ColorEmitter *emitter) {
  QMutexLocker locker(&m_mutex);
  if (m_emitter)
    m_emitter->done();

  (m_emitter = emitter)->init();
}


QSerialPortInfo ALCDeviceThread::details() { return m_details; }

void ALCDeviceThread::setContinueValue(bool value) {
  QMutexLocker locker(&m_mutex);
  m_continue = value;
}

bool ALCDeviceThread::continueValue() {
  QMutexLocker locker(&m_mutex);
  return m_continue;
}

void ALCDeviceThread::setDelay(int delay) {
  QMutexLocker locker(&m_mutex);
  m_delay = delay;
}

void ALCDeviceThread::setColorFormat(ALCDeviceThread::ColorFormat format) {
  QMutexLocker locker(&m_mutex);
  m_format = format;
}

void ALCDeviceThread::setBrigthness(double value) {
  QMutexLocker locker(&m_mutex);
  m_brightness = value;
}

void ALCDeviceThread::setBlueColorCorrection(double value) {
  QMutexLocker locker(&m_mutex);
  m_colorCorrection[Blue] = value;
}

void ALCDeviceThread::setGreenColorCorrection(double value) {
  QMutexLocker locker(&m_mutex);
  m_colorCorrection[Green] = value;
}

void ALCDeviceThread::setRedColorCorrection(double value) {
  QMutexLocker locker(&m_mutex);
  m_colorCorrection[Red] = value;
}

ALCDeviceThread::ColorFormat ALCDeviceThread::colorFormat() {
  QMutexLocker locker(&m_mutex);
  return m_format;
}

int ALCDeviceThread::delay() {
  QMutexLocker locker(&m_mutex);
  return m_delay;
}

double ALCDeviceThread::brightness() {
  QMutexLocker locker(&m_mutex);
  return m_brightness;
}

double ALCDeviceThread::blueColorCorrection() {
  QMutexLocker locker(&m_mutex);
  return m_colorCorrection[Blue];
}

double ALCDeviceThread::greenColorCorrection() {
  QMutexLocker locker(&m_mutex);
  return m_colorCorrection[Green];
}

double ALCDeviceThread::redColorCorrection() {
  QMutexLocker locker(&m_mutex);
  return m_colorCorrection[Red];
}
