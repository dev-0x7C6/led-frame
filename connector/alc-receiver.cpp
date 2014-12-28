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

#include "connector/alc-receiver.h"

ALCReceiver::ALCReceiver(Correctors::ALCColorCorrection::Type type)
  : Correctors::ALCColorCorrection(type),
    m_emitter(0) {
}

QString ALCReceiver::name() {
  return QString("default");
}

void ALCReceiver::connectEmitter(Emitters::ALCEmitter *emitter) {
  QMutexLocker locker(&m_mutex);

  if (m_emitter)
    m_emitter->done();

  if ((m_emitter = emitter))
    m_emitter->init();
}

Emitters::ALCEmitter *ALCReceiver::connectedEmitter() {
  QMutexLocker locker(&m_mutex);
  return m_emitter;
}
