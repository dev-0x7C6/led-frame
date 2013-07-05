#ifndef SERIALBACKEND_H
#define SERIALBACKEND_H

#include <QObject>

#include <QtSerialPort/QSerialPort>

class SerialBackend : public QObject
{
  Q_OBJECT
private:
  QSerialPort *m_port;


public:
  explicit SerialBackend(QObject *parent = 0);
  

private:
  void open (const QString &addr) {
    m_port = new QSerialPort(addr);
    m_port->setBaudRate(QSerialPort::Baud57600);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setStopBits(QSerialPort::OneStop);
    m_port->setParity(QSerialPort::NoParity);
  }

  void close() {
    delete m_port;
  }

};

#endif // SERIALBACKEND_H
