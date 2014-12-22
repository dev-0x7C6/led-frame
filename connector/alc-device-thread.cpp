/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
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
#include "emitters/alc-emitter.h"

#include <QElapsedTimer>

#define qb(b) qBlue(b)
#define qg(g) qGreen(g)
#define qr(r) qRed(r)

#include "classes/alc-strip-configuration.h"

ALCDeviceThread::ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent)
  : QThread(parent),
    ALCReceiver(),
    m_device(device),
    m_details(details),
    m_continue(true) {
  m_device->moveToThread(this);
  m_config = new ALCStripConfiguration();
  m_config->add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 30, true, RGB, 1.00);
  m_config->add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, RGB, 1.0);
  m_config->add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, RGB, 1.0);
  m_config->add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, RGB, 1.0);
  /* //LEDY JARKA
  config.add(ALCLedStrip::SourceBottom, ALCLedStrip::DestinationBottom, 6, true, GRB, 2.0);
  config.add(ALCLedStrip::SourceLeft, ALCLedStrip::DestinationLeft, 15, true, RGB, 1.0);
  config.add(ALCLedStrip::SourceTop, ALCLedStrip::DestinationTop, 30, true, RGB, 1.0);
  config.add(ALCLedStrip::SourceRight, ALCLedStrip::DestinationRight, 15, true, RGB, 1.0);
   */
}

ALCDeviceThread::~ALCDeviceThread() {
  delete m_config;
}

QString ALCDeviceThread::name() {
  QMutexLocker locker(&m_mutex);
  return m_details.systemLocation();
}

void ALCDeviceThread::run() {
  unsigned char data[2048];
  memset((char *)data, 0, sizeof(data));
  double rgbc[4];
  quint16 ptr = 0;
  ALCRuntimeSync sync;

  do {
    if (!m_emitter) {
      sync.wait(10);
      continue;
    }

    m_emitter->state(m_samples);
    rgbc[Brightness] = correction(ALCColorCorrection::Brightness, true);
    rgbc[Red] = correction(ALCColorCorrection::Red, true);
    rgbc[Green] = correction(ALCColorCorrection::Green, true);
    rgbc[Blue] = correction(ALCColorCorrection::Blue, true);
    QVector <int> *colors;
    ptr = 0;
    QList <ALCLedStrip *> strips = m_config->list();

    for (int ii = 0; ii < strips.count(); ++ii) {
      ALCLedStrip *strip = strips[ii];
      colors = m_samples.scaled((ALCColorSamples::Position)strip->source(), strip->count());
      const Format format = strip->colorFormat();
      const int size = colors->size();
      rgbc[Brightness] *= strip->brightness();

      if (strip->clockwise()) {
        for (int i = 0; i < size;)
          push(data, ptr, format, (*colors)[i++], rgbc);
      } else {
        for (int i = size - 1; i >= 0;)
          push(data, ptr, format, (*colors)[i--], rgbc);
      }

      delete colors;
    }

    m_device->write((char *)data, ptr);
    m_device->waitForBytesWritten(10);
    m_device->clear();
    sync.wait(100);
  } while (m_continue && m_device->error() == 0);

  if (m_device->isOpen() && m_device->isWritable())
    m_device->close();

  delete m_device;

  if (m_emitter)
    m_emitter->done();
}

QSerialPortInfo ALCDeviceThread::details() {
  return m_details;
}

void ALCDeviceThread::setContinueValue(bool value) {
  m_continue = value;
}

bool ALCDeviceThread::continueValue() {
  return m_continue;
}

void ALCDeviceThread::push(unsigned char *data, quint16 &ptr, Format format, quint32 color, double rgbc[]) {
  double brightness = rgbc[Brightness];

  switch (format) {
    case RGB:
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      break;

    case RBG:
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      break;

    case GRB:
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      break;

    case BRG:
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      break;

    case GBR:
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      break;

    case BGR:
      data[ptr++] = qMin(qb(color) * (rgbc[Blue] * brightness), 255.0);
      data[ptr++] = qMin(qg(color) * (rgbc[Green] * brightness), 255.0);
      data[ptr++] = qMin(qr(color) * (rgbc[Red] * brightness), 255.0);
      break;
  }
}
