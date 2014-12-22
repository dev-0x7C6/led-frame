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

#ifndef ALCIMAGEEMITTER_H
#define ALCIMAGEEMITTER_H

#include <QObject>
#include <QImage>
#include <QTimer>

#include "emitters/alc-emitter.h"

class ALCImageEmitter : public QObject, public ALCEmitter {
  Q_OBJECT
private:
  ALCColorSamples m_samples;
  QImage m_image;
  QString m_file;
  bool m_loaded;
  QTimer *m_timer;

public:
  explicit ALCImageEmitter(QObject *parent = 0);
  ~ALCImageEmitter();

  void fromFile(QString file);
  QString file();
  QString open();

  bool isLoaded();

  void pushState();
  void init();
  bool configure();
};

#endif // ALCIMAGEEMITTER_H
