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

#include "emitters/alc-image-emitter.h"

#include <QFileInfo>
#include <QFileDialog>

ALCImageEmitter::ALCImageEmitter(QObject *parent) :
  QObject(parent),
  ALCEmitter(),
  m_image(0),
  m_timer(new QTimer()) {
  m_type = EMITTER_IMAGE;
  connect(m_timer, &QTimer::timeout, this, &ALCImageEmitter::pushState);
  m_timer->setInterval(20);
  m_timer->start();
}

ALCImageEmitter::ALCImageEmitter() {
  delete m_timer;
}

QString ALCImageEmitter::file() {
  return m_file;
}

void ALCImageEmitter::fromFile(QString file) {
  m_image = QImage(file);
  m_file = file;
  m_loaded = !m_image.isNull();
  QVector < QImage> fragments[4];
  fragments[0].resize(64);
  fragments[1].resize(64);
  fragments[2].resize(64);
  fragments[3].resize(64);

  for(register int i = 0; i < 64; ++i)
    fragments[0][63 - i] = m_image.copy((m_image.width() / 64) * i, m_image.height() - 200, (m_image.width() / 64), 200);

  for(register int i = 0; i < 64; ++i)
    fragments[1][63 - i] = m_image.copy(0, (m_image.height() / 64) * i, 200, (m_image.height() / 64));

  for(register int i = 0; i < 64; ++i)
    fragments[2][i] = m_image.copy((m_image.width() / 64) * i, 0, (m_image.width() / 64), 200);

  for(register int i = 0; i < 64; ++i)
    fragments[3][i] = m_image.copy(m_image.width() - 200, (m_image.height() / 64) * i, 200, (m_image.height() / 64));

  int c = 0;
  double r = 0;
  double g = 0;
  double b = 0;
  int rgb = 0;

  for(register int k = 0; k < 4; ++k) {
    QVector < int> *colors = m_samples.get(ALCColorSamples::Position(k));

    for(register int i = 0; i < SAMPLE_RESOLUTION; ++i) {
      r = g = b = 0;
      c = 0;

      for(register int x = 0; x < fragments[k][i].width(); ++x)
        for(register int y = 0; y < fragments[k][i].height(); ++y) {
          rgb = fragments[k][i].pixel(x, y);
          r += qRed(rgb);
          g += qGreen(rgb);
          b += qBlue(rgb);
          c++;
        }

      r /= c;
      g /= c;
      b /= c;

      if(r > 255) r = 255;

      if(g > 255) g = 255;

      if(b > 255) b = 255;

      (*colors)[i] = qRgb(r, g, b);
    }
  }
}

void ALCImageEmitter::pushState() {
  setState(m_samples);
}

void ALCImageEmitter::init() {
  ALCEmitter::init();
}

bool ALCImageEmitter::configure() {
  return open().isEmpty();
}

QString ALCImageEmitter::open() {
  QFileInfo info(QFileDialog::getOpenFileName(0,
                 "Open Keylogger History File",
                 "*.jpg;*.png;*.xpm", "Images (*.png *.xpm *.jpg)"));

  if(info.exists() && info.isFile() && info.isReadable()) {
    fromFile(info.filePath());
  }

  return info.filePath();
}
