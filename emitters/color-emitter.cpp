#include "color-emitter.h"


ColorEmitter::ColorEmitter()
  :m_brightness(1.0),
   m_type(EMITTER_NOT_DEFINED)
{
  m_connectedCount = 0;
  for (register int i = 0; i < 64; ++i)
    m_colors << 0;
}

ColorEmitter::~ColorEmitter()
{

}

ColorEmitter::EmitterType ColorEmitter::type() const { return m_type; }

void ColorEmitter::setBrightness(double brightness) {
  QMutexLocker locker(&m_mutex);
  m_brightness = brightness;
}

double ColorEmitter::brightness() {
  QMutexLocker locker(&m_mutex);
  return m_brightness;
}

void ColorEmitter::init() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount++;
  m_colors.clear();
  for (register int i = 0; i < 64; ++i)
    m_colors << 0;
  qDebug() << this << " device connected, count: " << m_connectedCount;
}

void ColorEmitter::done() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount--;
  qDebug() << this << " device disconnected, count: " << m_connectedCount;
}

void ColorEmitter::setState(ColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
  m_samples.copy(samples);
}

void ColorEmitter::state(ColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
  samples.copy(m_samples);
}

//void ColorEmitter::setState(QList<QRgb> colors) {
//  QMutexLocker locker(&m_mutex);
//  m_colors = colors;
//}

//QList<QRgb> ColorEmitter::state() {
//  QMutexLocker locker(&m_mutex);
//  return m_colors;
//}
