#ifndef ALCSETTINGS_H
#define ALCSETTINGS_H
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

#include <QObject>

class QSettings;

class ALCSettings : public QObject {
  Q_OBJECT
private:
  QSettings *m_settings;
public:
  explicit ALCSettings(QObject *parent = 0);
  virtual ~ALCSettings();

  QSettings *settings();

  void load();
  void save();

  static ALCSettings *instance() {
    static ALCSettings settings;
    return &settings;
  }

};

#endif // ALCSETTINGS_H
