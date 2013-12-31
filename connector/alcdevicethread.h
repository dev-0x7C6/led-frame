#ifndef ACLDEVICETHREAD_H
#define ACLDEVICETHREAD_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>

#include <QByteArray>
#include <QBuffer>

#define qr(r) qRed(r)
#define qg(g) qGreen(g)
#define qb(b) qBlue(b)

#include <QElapsedTimer>

#include <QDebug>
#include <qmath.h>

#include "emitters/coloremitter.h"



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
  double m_brightness;

public:
  ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent = 0)
   :QThread(parent),
     m_device(device),
     m_emitter(0),
     m_details(details),
     m_continue(true),
     m_brightness(1.0)
  {
    m_device->moveToThread(this);

    for (register int i = 0; i < 32; ++i) {
      m_colors << 0x000000;
    }

  }

  QSerialPortInfo details() { return m_details; }

  void setContinueValue(bool value) {
    QMutexLocker locker(&m_mutex);
    m_continue = value;
  }

  bool continueValue() {
    QMutexLocker locker(&m_mutex);
    return m_continue;
  }



protected:
  void run();

public slots:
  void connectEmitter(ColorEmitter *emitter) {
    QMutexLocker locker(&m_mutex);
    if (m_emitter)
      m_emitter->done();

    (m_emitter = emitter)->init();
  }

  void setBrightness(double value);

};

#endif // ACLDEVICETHREAD_H
