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

#ifndef ALCDEVICEMANAGER_H
#define ALCDEVICEMANAGER_H

#include <QMutexLocker>
#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace AmbientLedConnector {

  namespace IDs {
    const QString Description = "Ambient Led Connector";
    const QString Manufacturer = "ALC";
  }

  namespace Transmision {
    const quint32 BaudRate = 500000;
  }
}

class ALCDeviceThread;

class ALCDeviceManager : public QObject {
  Q_OBJECT
private:
  QList < ALCDeviceThread*> m_threads;

public:
  explicit ALCDeviceManager(QObject *parent = 0);
  virtual ~ALCDeviceManager();

  ALCDeviceThread* device(int idx);

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void deviceThreadStarted();
  void deviceThreadFinished();
  
signals:
  void deviceConnected(ALCDeviceThread *thread);
  void deviceDisconnected(ALCDeviceThread *thread);

};

#endif // ALCDEVICEMANAGER_H
