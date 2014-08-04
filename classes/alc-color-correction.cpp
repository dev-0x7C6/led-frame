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
#include "classes/alc-weather-color-correction.h"

QList<ALCColorCorrection *> ALCColorCorrection::m_multipliers;

ALCColorCorrection::ALCColorCorrection(ALCColorCorrection::Type type) {
  QWriteLocker locker(&m_readWriteLock);
  m_colorCorrection[0] = 1.0;
  m_colorCorrection[1] = 1.0;
  m_colorCorrection[2] = 1.0;
  m_colorCorrection[3] = 1.0;
  m_format = GRB;
  m_type = type;
  m_enabled = true;
}

ALCColorCorrection::~ALCColorCorrection() {
}

bool ALCColorCorrection::enabled() {
  return m_enabled;
}

void ALCColorCorrection::setEnabled(bool enabled) {
  m_enabled = enabled;
}

ALCColorCorrection *ALCColorCorrection::weather() const {
  return m_weather;
}

void ALCColorCorrection::setWeather(ALCColorCorrection *weather) {
  m_weather = weather;
}

Format ALCColorCorrection::colorFormat() {
  return static_cast<Format>(m_format);
}

void ALCColorCorrection::setCorrection(ALCColorCorrection::Color color, double value) {
  m_colorCorrection[color] = value;
}

double ALCColorCorrection::correction(ALCColorCorrection::Color color, bool global) {
  double correction = m_colorCorrection[color];

  if (global)
    for (int i = 0; i < m_multipliers.count(); ++i) {
      if (m_multipliers[i] != this && m_multipliers[i]->enabled())
        correction *= m_multipliers[i]->correction(color);
    }

  return correction;
}

ALCColorCorrection *ALCColorCorrection::instance() {
  static ALCColorCorrection global(ALCColorCorrection::GLOBAL);
  static ALCColorCorrection weather(ALCColorCorrection::WEATHER);
  global.setWeather(&weather);
  return &global;
}

const QList<ALCColorCorrection *> &ALCColorCorrection::multipliers() {
  return m_multipliers;
}

void ALCColorCorrection::registerMultiplier(ALCColorCorrection *correction) {
  m_multipliers << correction;
}

void ALCColorCorrection::unregisterMultiplier(ALCColorCorrection *correction) {
  m_multipliers.removeAll(correction);
}

void ALCColorCorrection::correctionChanged() {
}
