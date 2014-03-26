#include "alc-color-correction.h"

#include <QMutexLocker>
#include <QMutex>

ALCColorCorrection::ALCColorCorrection() :
  m_mutex(new QMutex()) {
  QMutexLocker locker(m_mutex);
  m_colorCorrection[0] = 1.0d;
  m_colorCorrection[1] = 1.0d;
  m_colorCorrection[2] = 1.0d;
  m_format = GRB;
  m_brightness = 1.0d;
}

void ALCColorCorrection::setBrightness(double value) {
  QMutexLocker locker(m_mutex);
  m_brightness = value;
}

void ALCColorCorrection::setBlueCorrection(double value) {
  QMutexLocker locker(m_mutex);
  m_colorCorrection[Blue] = value;
}

void ALCColorCorrection::setGreenCorrection(double value) {
  QMutexLocker locker(m_mutex);
  m_colorCorrection[Green] = value;
}

void ALCColorCorrection::setRedCorrection(double value) {
  QMutexLocker locker(m_mutex);
  m_colorCorrection[Red] = value;
}

Format ALCColorCorrection::colorFormat() {
  QMutexLocker locker(m_mutex);
  return m_format;
}

double ALCColorCorrection::brightness(bool global) {
  QMutexLocker locker(m_mutex);
  return (global) ?
    m_brightness * ALCColorCorrection::instance()->brightness() :
    m_brightness;
}

double ALCColorCorrection::blueCorrection(bool global) {
  QMutexLocker locker(m_mutex);
  return (global) ?
    m_colorCorrection[Blue] * ALCColorCorrection::instance()->blueCorrection() :
    m_colorCorrection[Blue];
}

double ALCColorCorrection::greenCorrection(bool global) {
  QMutexLocker locker(m_mutex);
  return (global) ?
    m_colorCorrection[Green] * ALCColorCorrection::instance()->greenCorrection() :
    m_colorCorrection[Green];
}

double ALCColorCorrection::redCorrection(bool global) {
  QMutexLocker locker(m_mutex);
  return (global) ?
    m_colorCorrection[Red] * ALCColorCorrection::instance()->redCorrection() :
    m_colorCorrection[Red];
}

ALCColorCorrection *ALCColorCorrection::instance() {
  static ALCColorCorrection reference;
  return &reference;
}
