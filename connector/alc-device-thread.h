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
#include "classes/color-samples.h"

#define qr(r) qRed(r)
#define qg(g) qGreen(g)
#define qb(b) qBlue(b)

class ColorEmitter;

class ALCDeviceThread : public QThread
{
  Q_OBJECT
public:
  enum ColorFormat {
    RGB = 0x00,
    RBG,
    GRB,
    BRG,
    GBR,
    BGR
  };

  enum ColorId {
    Red = 0x00,
    Green,
    Blue
  };

private:
  QSerialPort *m_device;
  ColorSamples m_samples;
  ColorEmitter *m_emitter;
  QSerialPortInfo m_details;
  bool m_continue;
  QMutex m_mutex;
  QList < QRgb> m_colors;

  int m_delay;
  double m_brightness;
  double m_colorCorrection[3];
  ColorFormat m_format;

public:
  ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent = 0);
  QSerialPortInfo details();
  void setContinueValue(bool value);
  bool continueValue();

  void setBlueColorCorrection(double);
  void setBrigthness(double);
  void setColorFormat(ColorFormat format);
  void setDelay(int);
  void setGreenColorCorrection(double);
  void setRedColorCorrection(double);


  ColorFormat colorFormat();
  double blueColorCorrection();
  double brightness();
  double greenColorCorrection();
  double redColorCorrection();
  int delay();

protected:
  void run();

public slots:
  void connectEmitter(ColorEmitter *emitter);

};

#endif // ACLDEVICETHREAD_H
