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
    timer.start();
    if (m_emitter) {
      colors = m_emitter->state();
    }

    m_mutex.unlock();

    if (colors.isEmpty()) {
      msleep(10);
      continue;
    }

    ptr = 0;
    for (register int i = 0; i < colors.count()-4; ++i) {
      const quint32 color = colors[i];
      data[ptr++] = qg(color);
      data[ptr++] = qr(color);
      data[ptr++] = qb(color);
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
