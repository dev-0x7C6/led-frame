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

#ifndef ALCEMITTER_H
#define ALCEMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

#include "classes/alc-color-samples.h"
#include "classes/alc-color-correction.h"

class QTreeWidgetItem;

class ALCEmitter : public ALCColorCorrection {
public:
  enum EmitterType : quint8 {
    EMITTER_NOT_DEFINED = 0x00,
    EMITTER_SCREEN_CAPTURE,
    EMITTER_BLACKHOLE,
    EMITTER_ANIMATION,
    EMITTER_PLAIN_COLOR,
    EMITTER_IMAGE,
    EMITTER_END_ARRAY
  };

private:
  QString m_emitterName;
  ALCColorSamples m_safeSamples;
  QTreeWidgetItem *m_treeItem;

protected:
  EmitterType m_type;
  QMutex m_mutex;
  int m_connectedCount;

public:
  explicit ALCEmitter();
  virtual ~ALCEmitter();

  void setEmitterName(const QString &name);
  QString emitterName() const;


  EmitterType type() const;
  void setType(const EmitterType type) {
    m_type = type;
  }

  virtual void init();
  void done();
//  void setState(QList < QRgb> colors);
  void setState(ALCColorSamples &samples);
  void state(ALCColorSamples &samples);

  void setTreeItem(QTreeWidgetItem *item);
  QTreeWidgetItem *treeItem();

  virtual bool configure();
  virtual bool rename();
  virtual bool remove();


  //QList < QRgb> state();
};

#endif // ALCEMITTER_H
