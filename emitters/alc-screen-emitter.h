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

#ifndef ALCSCREENEMITTER_H
#define ALCSCREENEMITTER_H

#include "classes/alc-color-samples.h"
#include "emitters/alc-emitter.h"

#include <QList>
#include <QMutex>
#include <QPixmap>
#include <QRect>
#include <QRgb>
#include <QThread>

class QScreen;

enum ScreenFragments {
  Bottom = 0,
  Left,
  Top,
  Right
};

class ALCScreenEmitter : public QThread, public ALCEmitter {
  Q_OBJECT
public:
  ALCScreenEmitter(QObject *parent = 0);

private:
  ALCColorSamples m_samples;
  QScreen *m_screen;
  QString m_name;

  QRect m_captureArea;
  int m_chunkSize;
  int m_pixelSkip;
  int m_framerateLimit;
  bool m_quit;
  double m_marginProcent;

  QVector < int> *colors[4];

public slots:
  void setName(QString name);
  void setCaptureArea(QRect);
  void setChunkSize(int);
  void setPixelSkip(int);
  void setFramerateLimit(int);
  void setQuitState(bool);
  void setMarginProcent(double);

  QRect area();
  int chunk();
  int pixelSkip();
  int framerateLimit();
  double marginProcent();

  QString name();

public:
  bool configure();

protected:
  void run();

signals:
  void update(ALCColorSamples *samples);
  void updateStats(quint32 fps, double latency, double usage);

};

#endif // ALCSCREENEMITTER_H
