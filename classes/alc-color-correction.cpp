/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

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

ALCColorCorrection::~ALCColorCorrection() {
  delete m_mutex;
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
