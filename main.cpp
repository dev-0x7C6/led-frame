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

#include "mainwindow.h"
#include <QApplication>

#include "classes/alc-color-correction.h"
#include "classes/alc-weather-color-correction.h"
#include "managers/alc-emitter-manager.h"

const int applicationMajorVersion = 0;
const int applicationMinorVersion = 9;
const int applicationPatchVersion = 5;

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  application.setApplicationName("AmbientLedDriver");
  application.setApplicationVersion(QString("v%1.%2.%3").arg(
                                      QString::number(applicationMajorVersion),
                                      QString::number(applicationMinorVersion),
                                      QString::number(applicationPatchVersion)));
  application.setApplicationDisplayName(QString("%1 %2").arg(
                                          application.applicationName(),
                                          application.applicationVersion()));
  ALCEmitterManager::instance();
  ALCColorCorrection::instance()->registerMultiplier(ALCColorCorrection::instance());
  ALCColorCorrection::instance()->registerMultiplier(ALCWeatherColorCorrection::instance());
  MainWindow window;
  window.show();
  return application.exec();
}
