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

#include "alc-color-samples.h"
#include <unistd.h>

const int ALCColorSamples::Resolution = 30;

ALCColorSamples::ALCColorSamples() {
    m_samples[0].resize(ALCColorSamples::Resolution);
    m_samples[1].resize(ALCColorSamples::Resolution);
    m_samples[2].resize(ALCColorSamples::Resolution);
    m_samples[3].resize(ALCColorSamples::Resolution);
}

ALCColorSamples::~ALCColorSamples() { }

void ALCColorSamples::set(ALCColorSamples::Position pos, const QVector <int> &colors) {
    memcpy(reinterpret_cast <void *>(m_samples[pos].data()),
           reinterpret_cast <void *>(const_cast<int *>(colors.data())), colors.size() * sizeof(int));

}

QVector <int> *ALCColorSamples::get(ALCColorSamples::Position pos) {
    return &m_samples[pos];
}

QVector <int> *ALCColorSamples::scaled(ALCColorSamples::Position pos, int size) {
    QVector <int> *result = new QVector <int> (size);
    const QVector <int> &samples = m_samples[pos];
    double step = samples.size() / static_cast <double>(size);

    for (int i = 0; i < size; ++i)
        (*result)[i] = samples[i * step];

    (*result)[0] = samples.first();
    (*result)[size - 1] = samples.last();
    return result;
}

QVector <int> *ALCColorSamples::pscaled(ALCColorSamples::Position pos, int size) {
    QVector <int> *result = new QVector <int> (size);
    const QVector <int> &samples = m_samples[pos];
    double step = samples.size() / static_cast <double>(size);
    Q_UNUSED(step);
    return result;
}

quint16 ALCColorSamples::scale() {
    return ALCColorSamples::Resolution;
}

void ALCColorSamples::copy(const ALCColorSamples &ref) {
    for (int i = 0; i < SAMPLE_ARRAY; ++i)
        set(static_cast<Position>(i), ref.m_samples[i]);
}
