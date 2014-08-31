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

#ifndef ALCCOLORCORRECTION_H
#define ALCCOLORCORRECTION_H

#include <QObject>
#include <QList>
#include <QAtomicInt>

#include "classes/alc-safe-threading.h"

enum Color : unsigned char {
  Red   = 0x00,
  Blue  = 0x01,
  Green = 0x02
};

enum Format : unsigned char {
  RGB = 0x00,
  RBG = 0x01,
  GRB = 0x02,
  BRG = 0x03,
  GBR = 0x04,
  BGR = 0x05
};

class ALCColorCorrection : public ALCSafeThreading {
public:
  enum Type : unsigned char {
    GLOBAL = 0x00,
    WEATHER = 0x01,
    USER = 0x02
  };

  enum Color : unsigned char {
    Brightness = 0x00,
    Red = 0x01,
    Green = 0x02,
    Blue = 0x03
  };

protected:
  double m_colorCorrection[ALCColorCorrection::Blue + 1];
  bool m_enabled;
  Format m_format;
  Type m_type;


  static QList<ALCColorCorrection *> m_multipliers;
  ALCColorCorrection *m_weather;

public:
  explicit ALCColorCorrection(ALCColorCorrection::Type type = USER);
  ~ALCColorCorrection();

  void setColorFormat(const Format format);
  Format colorFormat();

  void setCorrection(ALCColorCorrection::Color color, double value);
  double correction(ALCColorCorrection::Color color, bool global = false);

  static ALCColorCorrection *instance();
  static const QList<ALCColorCorrection *> &multipliers();
  static void registerMultiplier(ALCColorCorrection *correction);
  static void unregisterMultiplier(ALCColorCorrection *correction);

  void clear();
  bool enabled();
  void setEnabled(bool enabled);

  ALCColorCorrection *weather() const;
  void setWeather(ALCColorCorrection *weather);

protected:
  virtual void correctionChanged();

};

#endif // ALCCOLORCORRECTION_H
