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

class ALCAnimationEmitter;
class ALCColorEmitter;
class ALCEmitter;
class ALCImageEmitter;
class ALCScreenEmitter;
class ALCSymulationWidget;
class QSettings;

class ALCEmitterManager : public QObject {
    Q_OBJECT
private:
    QList <ALCEmitter *> m_emitters[ALCEmitter::EMITTER_END_ARRAY];
    ALCSymulationWidget *m_symulation;

public:
    explicit ALCEmitterManager(QObject *parent = 0);
    virtual ~ALCEmitterManager();

    void add(ALCEmitter *emitter, ALCEmitter::EmitterType type);
    void remove(ALCEmitter *emitter);

    ALCAnimationEmitter *addALCAnimationEmitter(const QString &name);
    ALCColorEmitter *addALCColorEmitter(const QString &name);
    ALCImageEmitter *addALCImageEmitter(const QString &name);
    ALCScreenEmitter *addScreenCaptureEmitter(const QString &name);
    void addSymulation(ALCSymulationWidget *widget);

    static ALCEmitterManager *instance();
    const QList <ALCEmitter *> *emitters(ALCEmitter::EmitterType type);
    const QList <ALCEmitter *> allEmitters();

private:
    void readColorCorrection(QSettings *settings, ALCColorCorrection *correction);
    void writeColorCorrection(QSettings *settings, ALCColorCorrection *correction);

signals:
    void emitterListChanged();

};

#endif // ALCEmitterManager_H
