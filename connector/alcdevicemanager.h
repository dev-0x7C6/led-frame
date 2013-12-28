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

  ~ALCDeviceManager();

  ALCDeviceThread* device(int idx) {
    if (idx >= m_threads.count())
      return 0;

    return m_threads[idx];
  }

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
