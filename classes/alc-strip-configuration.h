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

#ifndef ALCSTRIPCONFIGURATION_H
#define ALCSTRIPCONFIGURATION_H

#include <QObject>
#include <QList>

#include "classes/alc-color-correction.h"

class ALCLedStrip {
public:
  enum Source {
    SourceBottom,
    SourceLeft,
    SourceTop,
    SourceRight
  };

  enum Destination {
    DestinationBottom,
    DestinationLeft,
    DestinationTop,
    DestinationRight
  };

private:
  int m_count;
  bool m_clockwise;
  Source m_source;
  Destination m_destination;
  Format m_colorFormat;
  double m_brightness;

public:
  void setClockwise(bool);
  bool clockwise() const;

  void setCount(int count);
  int count() const;

  void setColorFormat(Format format);
  Format colorFormat() const;

  void setSource(Source);
  Source source() const;

  void setDestination(Destination);
  Destination destination() const;


  void setBrightness(double value);
  double brightness();

};

class ALCStripConfiguration {
private:
  QList <ALCLedStrip *> m_strips;

public:
  explicit ALCStripConfiguration();

  void add(ALCLedStrip::Source source, ALCLedStrip::Destination destination,
           int count, bool clockwise = true, Format color = RGB, double brightness = 1.0);


  QList <ALCLedStrip *> list();

};

#endif // ALCSTRIPCONFIGURATION_H
