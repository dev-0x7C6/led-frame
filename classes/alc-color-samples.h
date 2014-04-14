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

#ifndef COLORSAMPLES_H
#define COLORSAMPLES_H

#include <QVector>

const int SAMPLE_RESOLUTION = 30;

class ALCColorSamples {
public:
  enum Position : quint8 {
    SAMPLE_BOTTOM,
    SAMPLE_LEFT,
    SAMPLE_TOP,
    SAMPLE_RIGHT,
    SAMPLE_ARRAY
  };

private:
  QVector < int> m_samples[SAMPLE_ARRAY];
  quint16 m_sampleCount;

public:
  explicit ALCColorSamples();
  virtual ~ALCColorSamples();

  void set(Position pos, QVector < int> &colors);
  QVector < int> *get(Position pos);
  QVector < int>* scaled(Position pos, int size);
  QVector < int>* pscaled(Position pos, int size);

  quint16 scale() { return SAMPLE_RESOLUTION; }

  void copy(ALCColorSamples &ref);
};

#endif // COLORSAMPLES_H
