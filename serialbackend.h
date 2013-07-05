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
    m_port->setBaudRate(QSerialPort::Baud115200);
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
    bits.resize(8*3);
    bits.fill(false, bits.size());

    int offset = 0;

    for (register int i = 8; i < 16; ++i) {
      bits.setBit(offset++, qRed(c[i]) > 100);
      bits.setBit(offset++, qGreen(c[i]) > 100);
      bits.setBit(offset++, qBlue(c[i]) > 100);
    }

    qDebug() << bits;

//    char hex[6];


//    for (register int i = 0; bits.count(); ++i) {
//      hex=

//    }

//    m_port->write(&hex, sizeof(hex));

  }

};

#endif // SERIALBACKEND_H
