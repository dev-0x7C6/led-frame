#ifndef COLOREMITTER_H
#define COLOREMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

class ColorEmitter
{
protected:
  QMutex m_mutex;
  int m_connectedCount;
  QList <QRgb> m_colors;

public:
  explicit ColorEmitter() {
    m_connectedCount = 0;
    for (register int i = 0; i < 32; ++i)
      m_colors << 0;
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

  QList < QRgb> state() {
    QMutexLocker locker(&m_mutex);
    return m_colors;
  }
  
};

#endif // COLOREMITTER_H
