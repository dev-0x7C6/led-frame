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
#include <QDebug>
QList<ALCColorCorrection *> ALCColorCorrection::m_multipliers;

ALCColorCorrection::ALCColorCorrection(ALCColorCorrection::Type type) {
  clear();
  QWriteLocker locker(&m_readWriteLock);
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

ALCColorCorrection::Format ALCColorCorrection::colorFormat() {
  return static_cast<Format>(m_format);
}

void ALCColorCorrection::setCorrection(ALCColorCorrection::Color color, double value) {
  switch (color) {
    case ALCColorCorrection::Red:
      m_r = value;
      break;

    case ALCColorCorrection::Green:
      m_g = value;
      break;

    case ALCColorCorrection::Blue:
      m_b = value;
      break;

    case ALCColorCorrection::Brightness:
      m_l = value;
      break;
  }
}

double ALCColorCorrection::correction(ALCColorCorrection::Color color, bool global) {
  double correction = 0;

  switch (color) {
    case ALCColorCorrection::Red:
      correction = m_r;
      break;

    case ALCColorCorrection::Green:
      correction = m_g;
      break;

    case ALCColorCorrection::Blue:
      correction = m_b;
      break;

    case ALCColorCorrection::Brightness:
      correction = m_l;
      break;
  }

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

void ALCColorCorrection::clear() {
  m_r = 1.0;
  m_g = 1.0;
  m_b = 1.0;
  m_l = 1.0;
  m_format = RGB;
}

void ALCColorCorrection::correctionChanged() {
}
