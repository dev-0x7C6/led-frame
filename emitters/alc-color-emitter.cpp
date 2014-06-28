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

#include "alc-color-emitter.h"

#include <QTimer>
#include <QTime>

ALCColorEmitter::ALCColorEmitter() :
  QObject(),
  ALCEmitter(),
  m_color(Qt::white),
  m_timer(new QTimer(this)) {
  srand(QTime::currentTime().msecsSinceStartOfDay());
  m_type = EMITTER_PLAIN_COLOR;
  connect(m_timer, &QTimer::timeout, this, &ALCColorEmitter::pushState);
  m_timer->setInterval(1000 / 15);
  m_timer->start();
}

ALCColorEmitter::~ALCColorEmitter() {}

void ALCColorEmitter::setColor(QColor color) {
  m_color = color;
}

QColor ALCColorEmitter::color() {
  return m_color;
}

double max(double value) {
  if (value > 255)
    return 255;

  return value;
}

void ALCColorEmitter::pushState() {
  QVector < int> samples(SAMPLE_RESOLUTION);
  int rgb = qRgb(max(m_color.red() * m_brightness),
                 max(m_color.green() * m_brightness),
                 max(m_color.blue() * m_brightness));

  for (register int i = 0; i < samples.size(); ++i)
    samples[i] = rgb;

  m_samples.set(ALCColorSamples::SAMPLE_TOP, samples);
  m_samples.set(ALCColorSamples::SAMPLE_LEFT, samples);
  m_samples.set(ALCColorSamples::SAMPLE_RIGHT, samples);
  m_samples.set(ALCColorSamples::SAMPLE_BOTTOM, samples);
  setState(m_samples);
}

#include <QColorDialog>

QColor ALCColorEmitter::open() {
  QColor color = QColorDialog::getColor(m_color);

  if (color.isValid())
    return (m_color = color);
  else
    return (m_color);
}

bool ALCColorEmitter::configure() {
  return open().isValid();
}
