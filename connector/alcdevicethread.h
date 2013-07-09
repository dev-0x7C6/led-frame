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

class ALCDeviceThread : public QThread
{
  Q_OBJECT
private:
  QSerialPort *m_device;
  QSerialPortInfo m_details;
  bool m_continue;
  QMutex m_mutex;
  QList < QRgb> m_colors;

public:
  ALCDeviceThread(QSerialPort *device, QSerialPortInfo details, QObject *parent = 0)
   :QThread(parent),
     m_device(device),
     m_details(details),
     m_continue(true)
  {
    m_device->moveToThread(this);

//    for (register int i = 0; i < 32/4; ++i) {
//      m_colors << 0xff0000;
//      m_colors << 0x00ff00;
//      m_colors << 0x0000ff;
//      m_colors << 0xff0000;
//    }

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

  int max(int value) {
    if (value > 255)
      return 255;
    return value;
  }

  void run() {

    quint8 byte;


    QList < QRgb> colors;

    int dynamicPwm = 4;

    QElapsedTimer timer;
    int bytesWritten = 0;
    int writeCount = 0;

    timer.start();
    m_mutex.lock();
    do {
      colors = m_colors;
      m_mutex.unlock();
      QByteArray array;
      QBuffer buffer(&array);
      buffer.open(QIODevice::WriteOnly);

      for (int pwm = 1; pwm <= dynamicPwm; ++pwm) {
        for (register int i = 0; i < colors.count(); ++i) {
          byte = 0x00;
          if (i == 0)
            byte |= 1 << 0x03;
          byte |= (qr(colors[i]) >= max(pwm*(256/dynamicPwm))) << 0x00;
          byte |= (qg(colors[i]) >= max(pwm*(256/dynamicPwm))) << 0x01;
          byte |= (qb(colors[i]) >= max(pwm*(256/dynamicPwm))) << 0x02;
          byte |= (qr(colors[++i]) >= max(pwm*(256/dynamicPwm))) << 0x04;
          byte |= (qg(colors[i]) >= max(pwm*(256/dynamicPwm))) << 0x05;
          byte |= (qb(colors[i]) >= max(pwm*(256/dynamicPwm))) << 0x06;
          buffer.write(reinterpret_cast< char*>(&byte), sizeof(byte));
        }
      }

      if (m_device->isWritable() && !array.isEmpty()) {
        m_device->write(array);
        m_device->flush();
      }

      bytesWritten += array.size();
      writeCount++;


      if (timer.hasExpired(1000)) {
        qDebug() << "Transfer rate:" << (bytesWritten/1024.0)
                 << "kb/s, Updates per second: "<< writeCount << "fps, Dynamic PWM: " << dynamicPwm;

        if (writeCount > 70)
          dynamicPwm++;
        if (writeCount < 60)
          dynamicPwm--;

        timer.restart();
        bytesWritten = 0;
        writeCount = 0;
      }

      m_mutex.lock();
    } while (m_continue && m_device->error() == QSerialPort::NoError);
    m_mutex.unlock();

    if (m_device->error() == QSerialPort::NoError) {
      byte = 0x81;
      m_device->write(reinterpret_cast< char*>(&byte), sizeof(byte));
      m_device->flush();
    }

    if (m_device->isOpen() && m_device->isWritable())
      m_device->close();

    delete m_device;
  }

public slots:
  void updateLeds(QList< QRgb> c) {
    QMutexLocker locker(&m_mutex);
    m_colors = c;
  }

};

#endif // ACLDEVICETHREAD_H
