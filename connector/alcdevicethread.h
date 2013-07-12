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

  void run() {

    register quint8 byte;


    QList < QRgb> colors;

    int dynamicPwm = 48;

    QElapsedTimer timer;
    int bytesWritten = 0;
    double writeCount = 0;
    double averageWriteCount = 0;

    double bytesWrittenAverage = 0;


    timer.start();
    m_mutex.lock();

    quint8 buffer[4096];
    quint16 bufferSize = 0;
    quint16 bufferPtr;

    do {
      colors = m_colors;
      m_mutex.unlock();

      bufferSize = 0;
      bufferPtr = 0;
      for (int pwm = 1; pwm <= dynamicPwm; ++pwm) {
        for (register int i = 0; i < colors.count(); ++i) {
          byte = 0x00;
          if (i == 0)
            byte |= 1 << 0x03;
          byte |= (qr(colors[i]) >= (pwm*(255/dynamicPwm))) << 0x00;
          byte |= (qg(colors[i]) >= (pwm*(255/dynamicPwm))) << 0x01;
          byte |= (qb(colors[i]) >= (pwm*(255/dynamicPwm))) << 0x02;
          byte |= (qr(colors[++i]) >= (pwm*(255/dynamicPwm))) << 0x04;
          byte |= (qg(colors[i]) >= (pwm*(255/dynamicPwm))) << 0x05;
          byte |= (qb(colors[i]) >= (pwm*(255/dynamicPwm))) << 0x06;
          buffer[bufferPtr++] = byte;
          ++bufferSize;
        }
      }

      writeCount++;

      if (m_device->isWritable() && bufferSize) {
        m_device->write(reinterpret_cast< char*>(buffer), bufferSize);
        bytesWritten += m_device->bytesToWrite();
        m_device->flush();
        bytesWritten -= m_device->bytesToWrite();
      }

      if (timer.hasExpired(1000)) {
        qDebug() << "Transfer rate:" << (bytesWritten/1024.0)
                 << "kb/s, Updates per second: "<< writeCount << "fps, Dynamic PWM: " << dynamicPwm;

        if (bytesWrittenAverage == 0)
         bytesWrittenAverage = bytesWritten;

        bytesWrittenAverage += qreal(bytesWritten);
        bytesWrittenAverage /= 2.0;

        dynamicPwm = dynamicPwm * (bytesWritten/bytesWrittenAverage);

        if (averageWriteCount == 0)
          averageWriteCount = writeCount;

        averageWriteCount += qreal(writeCount);
        averageWriteCount /= 2.0;

        dynamicPwm = dynamicPwm * (averageWriteCount/62.0);


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
