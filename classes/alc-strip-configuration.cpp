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

#include "alc-strip-configuration.h"

ALCStripConfiguration::ALCStripConfiguration() {
}

void ALCStripConfiguration::add(ALCLedStrip::Source source, ALCLedStrip::Destination destination,
                                int count, bool clockwise, ALCColorCorrection::Format color, double brightness) {
  ALCLedStrip *strip = new ALCLedStrip();
  strip->setSource(source);
  strip->setDestination(destination);
  strip->setCount(count);
  strip->setClockwise(clockwise);
  strip->setColorFormat(color);
  strip->setBrightness(brightness);
  m_strips << strip;
}

QList<ALCLedStrip *> ALCStripConfiguration::list() {
  return m_strips;
}

void ALCLedStrip::setClockwise(bool clockwise) {
  m_clockwise = clockwise;
}

bool ALCLedStrip::clockwise() const {
  return m_clockwise;
}

void ALCLedStrip::setCount(int count) {
  m_count = count;
}

int ALCLedStrip::count() const {
  return m_count;
}

void ALCLedStrip::setColorFormat(ALCColorCorrection::Format format) {
  m_colorFormat = format;
}

ALCColorCorrection::Format ALCLedStrip::colorFormat() const {
  return m_colorFormat;
}

void ALCLedStrip::setSource(ALCLedStrip::Source src) {
  m_source = src;
}

ALCLedStrip::Source ALCLedStrip::source() const {
  return m_source;
}

void ALCLedStrip::setDestination(ALCLedStrip::Destination dest) {
  m_destination = dest;
}

ALCLedStrip::Destination ALCLedStrip::destination() const {
  return m_destination;
}

void ALCLedStrip::setBrightness(double value) {
  m_brightness = value;
}

double ALCLedStrip::brightness() {
  return m_brightness;
}
