#include "alcdevicethread.h"


void ALCDeviceThread::run() {
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
  quint16 ptr = 0;


  do {
    if (m_emitter) {
      colors = m_emitter->state();
    }

    m_mutex.unlock();

    if (colors.isEmpty()) {
      msleep(10);
      continue;
    }

    ptr = 0;
    for (register int i = 0; i < colors.count(); ++i) {
      quint8 rgb[3];
      buffer[ptr++] = qg(colors[i]); // jak zmienisz na 0xff to beda sie palic wszystkie zielone,
      buffer[ptr++] = qr(colors[i]);
      buffer[ptr++] = qb(colors[i]);

      /*
        // zapalone wszystkie
        buffer[ptr++] = 0xff;
        buffer[ptr++] = 0xff;
        buffer[ptr++] = 0xff;
      */

      /*
        // wszystko zgaszone
        buffer[ptr++] = 0x00;
        buffer[ptr++] = 0x00;
        buffer[ptr++] = 0x00;
      */

      /*
        // wszystkie zielone na 100%
        buffer[ptr++] = 0xff; // zielony na 100%
        buffer[ptr++] = 0x00; // 0xff 100% dla czerwonego
        buffer[ptr++] = 0x00; // 0xff 100% dla blue
      */
    }

    m_device->write(reinterpret_cast < char*> (&buffer), ptr);
    m_device->flush();
    msleep(100);

//    bufferSize = 0;
//    bufferPtr = 0;
//    for (int pwm = 1; pwm <= dynamicPwm; ++pwm) {
//      for (register int i = 0; i < colors.count(); ++i) {
//        byte = 0x00;
//        if (i == 0)
//          byte |= 1 << 0x03;
//        byte |= ((qr(colors[i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x00;
//        byte |= ((qg(colors[i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x01;
//        byte |= ((qb(colors[i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x02;
//        byte |= ((qr(colors[++i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x04;
//        byte |= ((qg(colors[i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x05;
//        byte |= ((qb(colors[i])*m_brightness) >= (pwm*(255/dynamicPwm))) << 0x06;
//        buffer[bufferPtr++] = byte;
//        ++bufferSize;
//      }
//    }

//    writeCount++;
//    if (m_device->isWritable() && bufferSize) {
//      m_device->write(reinterpret_cast< char*>(buffer), bufferSize);
//      bytesWritten += m_device->bytesToWrite();
//      m_device->flush();
//      bytesWritten -= m_device->bytesToWrite();
//    }

//    if (timer.hasExpired(1000)) {
//      qDebug() << "Transfer rate:" << (bytesWritten/1024.0)
//               << "kb/s, Updates per second: "<< writeCount << "fps, Dynamic PWM: " << dynamicPwm;

//      if (bytesWrittenAverage == 0)
//        bytesWrittenAverage = bytesWritten;

//      bytesWrittenAverage += qreal(bytesWritten);
//      bytesWrittenAverage /= 2.0;

//      dynamicPwm = dynamicPwm * (bytesWritten/bytesWrittenAverage);

//      if (averageWriteCount == 0)
//        averageWriteCount = writeCount;

//      averageWriteCount += qreal(writeCount);
//      averageWriteCount /= 2.0;

//      dynamicPwm = dynamicPwm * (averageWriteCount/62.0);
//      timer.restart();
//      bytesWritten = 0;
//      writeCount = 0;
//    }

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

  if (m_emitter)
    m_emitter->done();
}
