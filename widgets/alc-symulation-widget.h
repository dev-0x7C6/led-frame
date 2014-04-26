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

#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "emitters/alc-emitter.h"
#include "classes/alc-color-samples.h"
#include "connector/alc-receiver.h"


#include <QWidget>
#include <QRgb>

#include <QVector>

class QQuickView;
class QQuickItem;

class ALCSymulationWidget :public QWidget, public ALCReceiver {
  Q_OBJECT
private:
  ALCColorSamples m_samples;
  QQuickItem *m_root;

  QObject *m_objs[4][8];
  QQuickItem *m_items[4][8];
  QQuickItem *m_monitor;
  QQuickView *m_view;

public:
  explicit ALCSymulationWidget(QWidget *parent = 0);
  virtual ~ALCSymulationWidget();

  void onShow();

  void createQmlMonitor();
  void freeQmlMonitor();
  void createQmlObjects(int size = 300);
  void freeQmlObjects();
  void resetQmlObjects();

private:
  void createQmlObject(int ii, int i, QQuickItem *item, QObject *obj, int size);
  QString name();

protected:
  void timerEvent(QTimerEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
