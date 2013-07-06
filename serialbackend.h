#ifndef SERIALBACKEND_H
#define SERIALBACKEND_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QRgb>
#include <QBitArray>
#include <QDebug>
#include <unistd.h>
#include <QtSerialPort/QSerialPortInfo>


class SerialBackend : public QObject
{
  Q_OBJECT
private:
  QSerialPort *m_port;
  unsigned char last[3];


public:
  explicit SerialBackend(QObject *parent = 0) {
    open("/dev/ttyUSB0");
  }
  
private:
  void open (const QString &addr) {

    qDebug() << QSerialPortInfo::availablePorts().count();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            qDebug() << "Name        : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
    }
    m_port = new QSerialPort(addr);


    m_port->setPortName(addr);
    m_port->setBaudRate(230400);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setParity(QSerialPort::NoParity);
    m_port->open(QIODevice::WriteOnly);
    // nie wiem czy po ? wiec dwa razy niech zyskam
    m_port->setBaudRate(230400);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setParity(QSerialPort::NoParity);


  }

  void close() {
    delete m_port;
  }

public slots:
  void updateLeds(QList< QRgb> c) {
    QBitArray bits;
    bits.resize(8*4);
    bits.fill(false, bits.size());

    int offset = 0;

    // Siatka dla 8 diod
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M
    // R G B M R G B M

    // 0xFF - 2 diody
    // 0xFF - 2 diody
    // 0xFF - 2 diody
    // 0xFF - 2 diody

    for (register int i = 8; i < 8+6; ++i) {
      bits.setBit(offset++, qRed(c[i]) > 100);   // R
      bits.setBit(offset++, qGreen(c[i]) > 100); // G
      bits.setBit(offset++, qBlue(c[i]) > 100);  // B
      bits.setBit(offset++, false);               // MOC
    }


    unsigned char hex[3];
    memset(reinterpret_cast< void*>(hex), 0, sizeof(hex));
    offset = -1;


    for (register int i = 0; i < bits.size(); ++i) {
      if (i % 8 == 0)
        offset++;

      hex[offset] |= bits[i] << (i % 8);
    }

    m_port->write(reinterpret_cast< char*>(hex), sizeof(hex));

  }

};

/*
  // qDebug() << QString::number(hex[0], 16) << QString::number(hex[1], 16)  << QString::number(hex[2], 16);

  //  }


   //  if (memcmp(reinterpret_cast< char*>(last), reinterpret_cast< char*>(hex), 6)) {

   // memcpy(reinterpret_cast< char*>(last), reinterpret_cast< char*>(hex), 6);
 */

#endif // SERIALBACKEND_H
