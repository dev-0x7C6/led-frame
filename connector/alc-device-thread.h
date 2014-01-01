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

#ifndef ACLDEVICETHREAD_H
#define ACLDEVICETHREAD_H

#include <QBuffer>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#define qr(r) qRed(r)
#define qg(g) qGreen(g)
#define qb(b) qBlue(b)

class ColorEmitter;

class ALCDeviceThread : public QThread
{
  Q_OBJECT
private:
  QSerialPort *m_device;
  ColorEmitter *m_emitter;
  QSerialPortInfo m_details;
  bool m_continue;
  QMutex m_mutex;
  QList < QRgb> m_colors;

public:
  ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent = 0);
  QSerialPortInfo details();
  void setContinueValue(bool value);
  bool continueValue();

protected:
  void run();

public slots:
  void connectEmitter(ColorEmitter *emitter);

};

#endif // ACLDEVICETHREAD_H
