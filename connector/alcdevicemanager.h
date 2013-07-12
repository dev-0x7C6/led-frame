#ifndef ALCDEVICEMANAGER_H
#define ALCDEVICEMANAGER_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutexLocker>
#include <QTimerEvent>

namespace AmbientLedConnectorIDs {
  const QString Description = "Ambient Led Connector";
  const QString Manufacturer = "ALC";
}

struct AmbientLedConnector {
  QSerialPort *device;
  QSerialPortInfo details;
};

#include <QDebug>

#include "connector/alcdevicethread.h"

class ALCDeviceManager : public QObject
{
  Q_OBJECT
private:
  QList < ALCDeviceThread*> m_threads;


public:
  explicit ALCDeviceManager(QObject *parent = 0) :QObject(parent) {
    startTimer(250);
  }

  ~ALCDeviceManager() {
    for (register int i = 0; i < m_threads.count(); ++i) {
      m_threads[i]->setContinueValue(false);
      m_threads[i]->wait();
      delete m_threads[i];
    }

    m_threads.clear();
  }

  ALCDeviceThread* device(int idx) {
    if (idx >= m_threads.count())
      return 0;

    return m_threads[idx];
  }

protected:
  void timerEvent(QTimerEvent *event) {
    QList < QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (register int i = 0; i < ports.count(); ++i) {
      if ((ports[i].manufacturer() != AmbientLedConnectorIDs::Manufacturer) ||
          (ports[i].description() != AmbientLedConnectorIDs::Description)) continue;

      QSerialPort *device = new QSerialPort(ports[i].portName());
      if (device->open(QIODevice::ReadWrite)) {
        device->setBaudRate(460800);//460800);
        ALCDeviceThread *thread = new ALCDeviceThread(device, ports[i]);
        connect(thread, SIGNAL(started()), this, SLOT(deviceThreadStarted()));
        connect(thread, SIGNAL(finished()), this, SLOT(deviceThreadFinished()));
        thread->start();

        qDebug() << "registred";

      } else
        delete device;

    }
  }

private slots:
  void deviceThreadStarted() {
    ALCDeviceThread *thread = dynamic_cast < ALCDeviceThread *>(sender());
    m_threads << thread;
    emit deviceConnected(thread);
  }

  void deviceThreadFinished() {
    ALCDeviceThread *thread = dynamic_cast < ALCDeviceThread *>(sender());
    QSerialPortInfo details = thread->details();
    m_threads.removeAll(thread);
    emit deviceDisconnected(thread);
    delete thread;
  }

  
signals:
  void deviceConnected(ALCDeviceThread *thread);
  void deviceDisconnected(ALCDeviceThread *thread);

};

#endif // ALCDEVICEMANAGER_H
