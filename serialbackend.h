#ifndef SERIALBACKEND_H
#define SERIALBACKEND_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QRgb>
#include <QBitArray>
#include <QDebug>


class SerialBackend : public QObject
{
  Q_OBJECT
private:
  QSerialPort *m_port;


public:
  explicit SerialBackend(QObject *parent = 0) {
    open("/dev/ttyUSB0");
  }
  
private:
  void open (const QString &addr) {
    m_port = new QSerialPort(addr);
    m_port->setBaudRate(QSerialPort::Baud57600);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setParity(QSerialPort::NoParity);

    m_port->setPortName(addr);
    m_port->open(QIODevice::ReadWrite);

  }

  void close() {
    delete m_port;
  }

public slots:
  void updateLeds(QList< QRgb> c) {
    QBitArray bits;
    bits.resize(16*3);
    bits.fill(false, bits.size());

    int offset = 0;

    for (register int i = 0; i < 16; ++i) {
      bits.setBit(offset++, qRed(c[i]) > 100);
      bits.setBit(offset++, qGreen(c[i]) > 100);
      bits.setBit(offset++, qBlue(c[i]) > 100);
    }

    qDebug() << bits;



    unsigned char hex[6];
    memset(reinterpret_cast< void*>(hex), 0, 6);
    int hexOffset = -1;


    for (register int i = 0; i < bits.size(); ++i) {
      if (i % 8 == 0)
          hexOffset++;

      hex[hexOffset] |= bits[i] << (i % 8);
    }
    qDebug() << QString::number(hex[0], 16) << QString::number(hex[1], 16)
            << QString::number(hex[2], 16) << QString::number(hex[3], 16)
            << QString::number(hex[4], 16)
            << QString::number(hex[5], 16);

    m_port->write(reinterpret_cast< char*>(hex), 6);

  }

};

#endif // SERIALBACKEND_H
