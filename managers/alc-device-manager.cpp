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

#include "managers/alc-device-manager.h"
#include "connector/alc-device-thread.h"

const int ALCDeviceManager::scanAfter = 250; // ms

ALCDeviceManager::ALCDeviceManager(QObject *parent)
  : QObject(parent),
    m_timerId(startTimer(ALCDeviceManager::scanAfter))
{
}

ALCDeviceManager::~ALCDeviceManager() {
}

void ALCDeviceManager::done()
{
  killTimer(m_timerId);

  for (int i = 0; i < m_threads.count(); ++i) {
    m_threads[i]->connectEmitter(0);
    m_threads[i]->setQuitState(true);
    m_threads[i]->wait();
    delete m_threads[i];
  }

  m_threads.clear();
}

ALCDeviceThread *ALCDeviceManager::device(int idx) const {
  if (idx >= m_threads.count())
    return 0;

  return m_threads[idx];
}

int ALCDeviceManager::count() const {
  return m_threads.count();
}

ALCDeviceManager *ALCDeviceManager::instance() {
  static ALCDeviceManager object;
  return &object;
}

void ALCDeviceManager::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event);
  QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

  for (int i = 0; i < ports.count(); ++i) {
    if ((ports[i].manufacturer() != AmbientLedConnector::IDs::Manufacturer) ||
        (ports[i].description() != AmbientLedConnector::IDs::Description)) continue;

    QSerialPort *device = new QSerialPort(ports[i].portName());

    if (device->open(QIODevice::ReadWrite)) {
      device->setBaudRate(AmbientLedConnector::Transmision::BaudRate);
      device->setFlowControl(QSerialPort::NoFlowControl);
      device->setParity(QSerialPort::NoParity);
      device->setDataBits(QSerialPort::Data8);
      device->setStopBits(QSerialPort::OneStop);
      ALCDeviceThread *thread = new ALCDeviceThread(device, ports[i]);
      connect(thread, &ALCDeviceThread::started, this, &ALCDeviceManager::deviceThreadStarted);
      connect(thread, &ALCDeviceThread::finished, this, &ALCDeviceManager::deviceThreadFinished);
      thread->start();
    } else
      delete device;
  }
}

void ALCDeviceManager::deviceThreadStarted() {
  ALCDeviceThread *thread = dynamic_cast <ALCDeviceThread *>(sender());
  m_threads << thread;
  emit deviceConnected(thread);
}

void ALCDeviceManager::deviceThreadFinished() {
  ALCDeviceThread *thread = dynamic_cast <ALCDeviceThread *>(sender());
  QSerialPortInfo details = thread->details();
  m_threads.removeAll(thread);
  emit deviceDisconnected(thread);
  delete thread;
}
