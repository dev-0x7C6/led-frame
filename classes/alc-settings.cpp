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

#include "alc-settings.h"
#include "correctors/alc-color-correction.h"

#include <QSettings>

ALCSettings::ALCSettings() :
  m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver")) {
  load();
}

ALCSettings::~ALCSettings() {
  save();
  delete m_settings;
}

QSettings *ALCSettings::settings() {
  return m_settings;
}

void ALCSettings::load() {
  //  m_settings->beginGroup("GlobalColorCorrection");
  //  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Brightness, m_settings->value("Brightness", 1.0).toDouble());
  //  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Blue, m_settings->value("Blue", 1.0).toDouble());
  //  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Green, m_settings->value("Green", 1.0).toDouble());
  //  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Red, m_settings->value("Red", 1.0).toDouble());
  //  m_settings->endGroup();
}

void ALCSettings::save() {
  //  m_settings->beginGroup("GlobalColorCorrection");
  //  m_settings->setValue("Brightness", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Brightness));
  //  m_settings->setValue("Blue", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Blue));
  //  m_settings->setValue("Green", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Green));
  //  m_settings->setValue("Red", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Red));
  //  m_settings->endGroup();
}
