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

#ifndef ALCCOLORCORRECTIONWIDGET_H
#define ALCCOLORCORRECTIONWIDGET_H

#include <QWidget>

namespace Ui { class ALCColorCorrectionWidget; }

class ALCColorCorrection;

class ALCColorCorrectionWidget : public QWidget {
  Q_OBJECT
private:
  Ui::ALCColorCorrectionWidget *ui;
  ALCColorCorrection *m_correction;

public:
  explicit ALCColorCorrectionWidget(QWidget *parent = 0);
  ~ALCColorCorrectionWidget();

  void setColorCorrection(ALCColorCorrection *);
  void reload();
  void restore();

private:
  void brightnessSliderChanged(int);
  void blueSliderChanged(int);
  void greenSliderChanged(int);
  void redSliderChanged(int);

  void setDefaultBrightness();
  void setDefaultRed();
  void setDefaultGreen();
  void setDefaultBlue();

};

#endif // ALCCOLORCORRECTIONWIDGET_H
