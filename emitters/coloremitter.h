#ifndef COLOREMITTER_H
#define COLOREMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

class ColorEmitter
{
private:
  QList <QRgb> m_colors;
  double m_brightness;

protected:
  QMutex m_mutex;
  int m_connectedCount;

public:
  explicit ColorEmitter() : m_brightness(1.0) {
    m_connectedCount = 0;
    for (register int i = 0; i < 64; ++i)
      m_colors << 0;
  }

  void setBrightness(double brightness) {
    QMutexLocker locker(&m_mutex);
    m_brightness = brightness;
  }

  double brightness() {
    QMutexLocker locker(&m_mutex);
    return m_brightness;
  }

  
  void init() {
    QMutexLocker locker(&m_mutex);
    m_connectedCount++;
    qDebug() << this << " device connected, count: " << m_connectedCount;
  }

  void done() {
    QMutexLocker locker(&m_mutex);
    m_connectedCount--;
    qDebug() << this << " device disconnected, count: " << m_connectedCount;
  }

  void setState(QList < QRgb> colors) {
    QMutexLocker locker(&m_mutex);
    m_colors = colors;
  }

  QList < QRgb> state() {
    QMutexLocker locker(&m_mutex);
    return m_colors;
  }
  
};

#endif // COLOREMITTER_H
