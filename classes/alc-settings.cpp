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
#include "classes/alc-color-correction.h"

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
    m_settings->beginGroup("GlobalColorCorrection");
    ALCColorCorrection::instance()->setCorrection(ALCColorCorrection::Brightness, m_settings->value("Brightness", 1.0).toDouble());
    ALCColorCorrection::instance()->setCorrection(ALCColorCorrection::Red, m_settings->value("Blue", 1.0).toDouble());
    ALCColorCorrection::instance()->setCorrection(ALCColorCorrection::Green, m_settings->value("Green", 1.0).toDouble());
    ALCColorCorrection::instance()->setCorrection(ALCColorCorrection::Blue, m_settings->value("Red", 1.0).toDouble());
    m_settings->endGroup();
}

void ALCSettings::save() {
    m_settings->beginGroup("GlobalColorCorrection");
    m_settings->setValue("Brightness", ALCColorCorrection::instance()->correction(ALCColorCorrection::Brightness));
    m_settings->setValue("Blue", ALCColorCorrection::instance()->correction(ALCColorCorrection::Red));
    m_settings->setValue("Green", ALCColorCorrection::instance()->correction(ALCColorCorrection::Green));
    m_settings->setValue("Red", ALCColorCorrection::instance()->correction(ALCColorCorrection::Blue));
    m_settings->endGroup();
}
