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

#ifndef ACLDEVICETHREAD_H
#define ACLDEVICETHREAD_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "classes/alc-color-samples.h"
#include "connector/alc-receiver.h"

class ALCEmitter;
class ALCColorSamples;
class ALCStripConfiguration;

class ALCDeviceThread : public QThread, public ALCReceiver {
  Q_OBJECT
private:
  ALCStripConfiguration *m_config;
  QSerialPort *m_device;
  ALCColorSamples m_samples;
  QSerialPortInfo m_details;
  bool m_continue;

public:
  explicit ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent = 0);
  virtual ~ALCDeviceThread();

  QString name();

  QSerialPortInfo details();
  void setContinueValue(bool value);
  bool continueValue();

private:
  void push(unsigned char *data, quint16 &ptr, Format format, quint32 color, double rgbc[]);

protected:
  void run();
};

#endif // ACLDEVICETHREAD_H
