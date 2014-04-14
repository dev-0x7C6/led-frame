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

class QMutex;

class ALCColorCorrection {
protected:
  double m_colorCorrection[3];
  double m_brightness;
  Format m_format;
  QMutex *m_mutex;

public:
  explicit ALCColorCorrection();

  void setColorFormat(Format format);
  void setBrightness(double value);
  void setBlueCorrection(double value);
  void setGreenCorrection(double value);
  void setRedCorrection(double value);

  Format colorFormat();
  double brightness(bool global = false);
  double blueCorrection(bool global = false);
  double greenCorrection(bool global = false);
  double redCorrection(bool global = false);

  static ALCColorCorrection *instance();
};

#endif // ALCCOLORCORRECTION_H
