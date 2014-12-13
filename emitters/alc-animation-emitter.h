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

#ifndef ALCANIMATIONEMITTER_H
#define ALCANIMATIONEMITTER_H

#include <QObject>
#include <QColor>
#include <QRgb>

#include <QPropertyAnimation>
#include "emitters/alc-emitter.h"

class ALCAnimationEmitter : public QObject, public ALCEmitter {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    enum AnimationType : quint8 {
        Blink,
        Glow,
        Rotation
    };

private:
    QPropertyAnimation *m_animation;
    QColor m_color;

    QList <QRgb> m_colorStream;
    ALCColorSamples m_samples;
    AnimationType m_animationType;
    quint64 m_blink;

public:
    explicit ALCAnimationEmitter();
    virtual ~ALCAnimationEmitter();

    bool open();
    virtual bool configure();

    void rotatePalette(int msecs = 1000);

private:
    const QColor &color();
    void setColor(const QColor &color);

    void glow();

protected:
    void timerEvent(QTimerEvent *);
};

#endif // ALCANIMATIONEMITTER_H
