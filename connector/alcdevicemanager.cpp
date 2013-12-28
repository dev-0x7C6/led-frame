#include "alcdevicemanager.h"

ALCDeviceManager::~ALCDeviceManager() {
  for (register int i = 0; i < m_threads.count(); ++i) {
    m_threads[i]->setContinueValue(false);
    m_threads[i]->wait();
    delete m_threads[i];
  }

  m_threads.clear();
}

void ALCDeviceManager::timerEvent(QTimerEvent *event) {
  QList < QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
  for (register int i = 0; i < ports.count(); ++i) {
    if ((ports[i].manufacturer() != AmbientLedConnectorIDs::Manufacturer) ||
        (ports[i].description() != AmbientLedConnectorIDs::Description)) continue;

    QSerialPort *device = new QSerialPort(ports[i].portName());
    if (device->open(QIODevice::ReadWrite)) {
      device->setBaudRate(500000);
      ALCDeviceThread *thread = new ALCDeviceThread(device, ports[i]);
      connect(thread, SIGNAL(started()), this, SLOT(deviceThreadStarted()));
      connect(thread, SIGNAL(finished()), this, SLOT(deviceThreadFinished()));
      thread->start();

      qDebug() << "registred";

    } else
      delete device;

  }
}

void ALCDeviceManager::deviceThreadStarted() {
  ALCDeviceThread *thread = dynamic_cast < ALCDeviceThread *>(sender());
  m_threads << thread;
  emit deviceConnected(thread);
}

void ALCDeviceManager::deviceThreadFinished() {
  ALCDeviceThread *thread = dynamic_cast < ALCDeviceThread *>(sender());
  QSerialPortInfo details = thread->details();
  m_threads.removeAll(thread);
  emit deviceDisconnected(thread);
  delete thread;
}
