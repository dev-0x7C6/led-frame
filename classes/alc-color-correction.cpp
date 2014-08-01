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

ALCColorCorrection::ALCColorCorrection() {
  QWriteLocker locker(&m_readWriteLock);
  m_colorCorrection[0] = 1.0;
  m_colorCorrection[1] = 1.0;
  m_colorCorrection[2] = 1.0;
  m_format = GRB;
  m_brightness = 1.0;
}

ALCColorCorrection::~ALCColorCorrection() {
}

void ALCColorCorrection::setBrightness(const double value) {
  QWriteLocker locker(&m_readWriteLock);
  m_brightness = value;
}

void ALCColorCorrection::setBlueCorrection(const double value) {
  QWriteLocker locker(&m_readWriteLock);
  m_colorCorrection[Blue] = value;
}

void ALCColorCorrection::setGreenCorrection(const double value) {
  QWriteLocker locker(&m_readWriteLock);
  m_colorCorrection[Green] = value;
}

void ALCColorCorrection::setRedCorrection(const double value) {
  QWriteLocker locker(&m_readWriteLock);
  m_colorCorrection[Red] = value;
}

Format ALCColorCorrection::colorFormat() {
  QReadLocker locker(&m_readWriteLock);
  return m_format;
}

double ALCColorCorrection::brightness(bool global) {
  QReadLocker locker(&m_readWriteLock);
  return (global) ?
         m_brightness * ALCColorCorrection::instance()->brightness() :
         m_brightness;
}

double ALCColorCorrection::blueCorrection(bool global) {
  QReadLocker locker(&m_readWriteLock);
  return (global) ?
         m_colorCorrection[Blue] * ALCColorCorrection::instance()->blueCorrection() :
         m_colorCorrection[Blue];
}

double ALCColorCorrection::greenCorrection(bool global) {
  QReadLocker locker(&m_readWriteLock);
  return (global) ?
         m_colorCorrection[Green] * ALCColorCorrection::instance()->greenCorrection() :
         m_colorCorrection[Green];
}

double ALCColorCorrection::redCorrection(bool global) {
  QReadLocker locker(&m_readWriteLock);
  return (global) ?
         m_colorCorrection[Red] * ALCColorCorrection::instance()->redCorrection() :
         m_colorCorrection[Red];
}

ALCColorCorrection *ALCColorCorrection::instance() {
  static ALCColorCorrection reference;
  return &reference;
}
