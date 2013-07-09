#ifndef SERIALBACKEND_H
#define SERIALBACKEND_H

#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <QRgb>
#include <QBitArray>
#include <QDebug>
#include <unistd.h>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>

#include <QMutex>
#include <QMutexLocker>

//namespace AmbientLedConnectorIDs {
//  const QString Description = "Ambient Led Connector";
//  const QString Manufacturer = "ALC";
//}

//struct AmbientLedConnector {
//  QSerialPort *device;
//  QSerialPortInfo details;
//};

#include <QBuffer>
#include <QElapsedTimer>

#define qr(r) qRed(r)
#define qg(g) qGreen(g)
#define qb(b) qBlue(b)


class SerialBackend : public QThread
{
  Q_OBJECT
private:
//  QList < AmbientLedConnector> m_devices;
  QList< QRgb> m_colors;
  QMutex m_mutex;

  int m_master;
  bool m_continue;

  unsigned char last[3];

  int max(int value) {
    if (value > 255)
      return 255;
    return value;
  }

public:
  explicit SerialBackend(QObject *parent = 0) :QThread(parent) {
    m_master = -1;
    m_continue = true;
    moveToThread(this);
  }
  

protected:

  void run() {

    do {
//      foreach (QSerialPortInfo details, QSerialPortInfo::availablePorts()) {
//         if ((details.manufacturer() != AmbientLedConnectorIDs::Manufacturer) ||
//             (details.description() != AmbientLedConnectorIDs::Description)) continue;

//         bool next = false;

//         for (register int i = 0; i < m_devices.count(); ++i)
//           if (m_devices[i].details.vendorIdentifier()  == details.vendorIdentifier() &&
//               m_devices[i].details.manufacturer() == details.manufacturer() &&
//               m_devices[i].details.description() == details.description() &&
//               m_devices[i].details.systemLocation() == details.systemLocation())
//             next = true;

//         if (next)
//           continue;
//      }



    } while (m_continue);



//      AmbientLedConnector connector;
//      connector.details = details;
//      connector.device = new QSerialPort(details.portName());
//      if (connector.device->open(QIODevice::WriteOnly)) {
//        connector.device->setBaudRate(230400);
//        m_devices << connector;
//        qDebug() << "Device: " << connector.details.portName() << "- connected.";
//      } else {
//        qDebug() << "Device: " << connector.details.portName() << "- can't connect. Error: " << connector.device->error();
//        delete connector.device;
//      }

//      if (!m_devices.isEmpty())
//        m_master = 0;

//      quint8 byte;
//      QElapsedTimer timer;


//      int bytesWritten = 0;
//      int writeCount = 0;
//      timer.start();

//      do {
//        m_mutex.lock();
//        QList < QRgb> colors = m_colors;
//        m_mutex.unlock();

//        QByteArray array;
//        QBuffer buffer(&array);
//        buffer.open(QIODevice::WriteOnly);

//        qDebug() <<  m_devices[m_master].device->bytesAvailable();



//        bytesWritten += array.size();
//        writeCount++;

//        if (m_devices[m_master].device->isWritable()) {
//          m_devices[m_master].device->write(array);
//          m_devices[m_master].device->flush();
//        } else
//          msleep(10); // don't eat cpu time



//      } while (m_continue);



  }

public slots:
  void updateLeds(QList< QRgb> c) {
    QMutexLocker locker(&m_mutex);
    m_colors = c;
  }

};

#endif // SERIALBACKEND_H
