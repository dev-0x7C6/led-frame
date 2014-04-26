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

#ifndef ALCRECEIVER_H
#define ALCRECEIVER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include "classes/alc-color-correction.h"
#include "emitters/alc-emitter.h"



class ALCReceiver :public ALCColorCorrection {
protected:
  ALCEmitter *m_emitter;

public:
  ALCReceiver();

  virtual QString name();

  void connectEmitter(ALCEmitter *emitter);
  ALCEmitter *connectedEmitter();
};

#endif // ALCRECEIVER_H
