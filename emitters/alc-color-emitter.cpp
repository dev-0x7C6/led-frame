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

#include "alc-color-emitter.h"

#include <QColorDialog>
#include <QTime>
#include <QTimer>

ALCColorEmitter::ALCColorEmitter() :
    QObject(),
    ALCEmitter(),
    m_color(Qt::white),
    m_timer(new QTimer(this)) {
    srand(QTime::currentTime().msecsSinceStartOfDay());
    m_type = EMITTER_PLAIN_COLOR;
    connect(m_timer, &QTimer::timeout, this, &ALCColorEmitter::pushState);
    m_timer->setInterval(1000 / 15);
    m_timer->start();
}

ALCColorEmitter::~ALCColorEmitter() {}

void ALCColorEmitter::setColor(const QColor &color) {
    m_color = color;
}

const QColor &ALCColorEmitter::color() {
    return m_color;
}

void ALCColorEmitter::pushState() {
    QVector <int> samples(ALCColorSamples::Resolution);
    int rgb = qRgb(qMin(m_color.red() * correction(ALCColorCorrection::Red), 255.0),
                   qMin(m_color.green() * correction(ALCColorCorrection::Green), 255.0),
                   qMin(m_color.blue() * correction(ALCColorCorrection::Blue), 255.0));

    for (int i = 0; i < samples.size(); ++i)
        samples[i] = rgb;

    m_samples.set(ALCColorSamples::SAMPLE_TOP, samples);
    m_samples.set(ALCColorSamples::SAMPLE_LEFT, samples);
    m_samples.set(ALCColorSamples::SAMPLE_RIGHT, samples);
    m_samples.set(ALCColorSamples::SAMPLE_BOTTOM, samples);
    setState(m_samples);
}

const QColor &ALCColorEmitter::open() {
    QColor color = QColorDialog::getColor(m_color);

    if (color.isValid())
        return (m_color = color);
    else
        return (m_color);
}

bool ALCColorEmitter::configure() {
    return open().isValid();
}
