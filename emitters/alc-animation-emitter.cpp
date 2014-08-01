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

#include <QMessageBox>
#include "emitters/alc-animation-emitter.h"

ALCAnimationEmitter::ALCAnimationEmitter() :
  QObject(),
  ALCEmitter(),
  m_animation(0) {
  m_type = EMITTER_ANIMATION;
  rotatePalette();
  startTimer(1000 / 60);

  for (int i = 0; i < 256; ++i)
    m_colorStream << qRgb(0, 0, 0);

  m_animationType = Rotation;
}

ALCAnimationEmitter::~ALCAnimationEmitter() {
  if (m_animation)
    delete m_animation;
}

const QColor &ALCAnimationEmitter::color() {
  return m_color;
}

void ALCAnimationEmitter::setColor(const QColor &color) {
  m_color = color;
}

bool ALCAnimationEmitter::open() {
  QMessageBox::warning(0, "Warning", "To implement.", QMessageBox::Ok);
  return false;
}

bool ALCAnimationEmitter::configure() {
  QMessageBox::information(0, "Information", "Animation configurator is under development.", QMessageBox::Ok);
  return false;
}

void ALCAnimationEmitter::rotatePalette() {
  if (m_animation)
    delete m_animation;

  m_animation = new QPropertyAnimation(this, "color");
  m_animation->setDuration(750);
  m_animation->setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
  m_animation->setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
  m_animation->setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
  m_animation->setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
  m_animation->setLoopCount(-1);
  m_animation->start();
  ;
}

void ALCAnimationEmitter::glow() {
  if (m_animation)
    delete m_animation;

  m_animation = new QPropertyAnimation(this, "color");
  m_animation->setDuration(10000);
  m_animation->setKeyValueAt(0.000, QColor::fromRgbF(0.0, 0.0, 0.0));
  m_animation->setKeyValueAt(0.500, QColor::fromRgbF(0.5, 1, 0));
  m_animation->setKeyValueAt(1.000, QColor::fromRgbF(0.0, 0.0, 0.0));
  m_animation->setLoopCount(-1);
  m_animation->start();
}

void ALCAnimationEmitter::timerEvent(QTimerEvent *) {
  if (!m_connectedCount)
    return;

  m_blink++;
  int rgb;

  switch (m_animationType) {
    case Rotation: {
      if (m_colorStream.count() == 256)
        m_colorStream.removeFirst();

      m_colorStream << qRgb(qMin(qRed(m_color.rgb()), 0xFF),
                            qMin(qGreen(m_color.rgb()), 0xFF),
                            qMin(qBlue(m_color.rgb()), 0xFF));
      QListIterator <QRgb> it(m_colorStream);

      for (int p = 0; p < 4; ++p) {
        QVector <int> *vec = m_samples.get(static_cast <ALCColorSamples::Position>(p));

        for (int i = 0; i < ALCColorSamples::Resolution; ++i)
          (*vec)[i] = it.next();
      }
    }
    break;

    case Glow:
      if (m_blink < 2)
        rgb = qRgb(qMin(qRed(m_color.rgb()), 0xFF),
                   qMin(qGreen(m_color.rgb()), 0xFF),
                   qMin(qBlue(m_color.rgb()), 0xFF));
      else
        rgb = qRgb(0, 0, 0);

      if (m_blink > 3)
        m_blink = 0;

      for (int p = 0; p < 4; ++p) {
        QVector <int> *vec = m_samples.get(static_cast <ALCColorSamples::Position>(p));

        for (int i = 0; i < ALCColorSamples::Resolution; ++i)
          (*vec)[i] = rgb;
      }

      break;

    default:
      break;
  }

  setState(m_samples);
}
