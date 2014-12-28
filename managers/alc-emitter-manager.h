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

#ifndef ALCEmitterManager_H
#define ALCEmitterManager_H

#include <QObject>
#include "emitters/alc-emitter.h"

namespace Emitters {
  class ALCAnimationEmitter;
  class ALCColorEmitter;
  class ALCEmitter;
  class ALCImageEmitter;
  class ALCScreenEmitter;
}

class ALCSymulationWidget;
class QSettings;

class ALCEmitterManager : public QObject {
  Q_OBJECT
private:
  QList <Emitters::ALCEmitter *> m_emitters[static_cast<int>(Emitters::ALCEmitter::Type::Last)];
  ALCSymulationWidget *m_symulation;

public:
  explicit ALCEmitterManager(QObject *parent = 0);
  virtual ~ALCEmitterManager();

  void done();

  void add(Emitters::ALCEmitter *emitter, Emitters::ALCEmitter::Type type);
  void remove(Emitters::ALCEmitter *emitter);

  Emitters::ALCAnimationEmitter *addALCAnimationEmitter(const QString &name);
  Emitters::ALCColorEmitter *addALCColorEmitter(const QString &name);
  Emitters::ALCImageEmitter *addALCImageEmitter(const QString &name);
  Emitters::ALCScreenEmitter *addScreenCaptureEmitter(const QString &name);
  void addSymulation(ALCSymulationWidget *widget);

  static ALCEmitterManager *instance();
  const QList <Emitters::ALCEmitter *> *emitters(Emitters::ALCEmitter::Type type);
  const QList <Emitters::ALCEmitter *> allEmitters();

private:
  void readColorCorrection(QSettings *settings, Correctors::ALCColorCorrection *correction);
  void writeColorCorrection(QSettings *settings, Correctors::ALCColorCorrection *correction);

signals:
  void emitterListChanged();

};

#endif // ALCEmitterManager_H
