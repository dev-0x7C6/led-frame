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

#ifndef ALCCOLORCORRECTIONMANAGER_H
#define ALCCOLORCORRECTIONMANAGER_H

#include <QList>

#include "correctors/alc-color-correction.h"

struct ALCColorCorrectionValues;

namespace Correctors {

  class ALCColorCorrection;
  struct ALCColorCorrectionValues;

  class  ALCColorCorrectionManager {
  private:
    static const QList <ALCColorCorrection *> list;

  public:
    ALCColorCorrectionManager();
    ~ALCColorCorrectionManager();

    static ALCColorCorrectionManager *instance();

    ALCColorCorrectionValues correction();
    double correction(ALCColorCorrection::Color color);

    void init();
    void done();

  private:
    void save(ALCColorCorrection *correction, QString name);
    void load(ALCColorCorrection *correction, QString name);

  };

}

#endif // ALCCOLORCORRECTIONMANAGER_H
