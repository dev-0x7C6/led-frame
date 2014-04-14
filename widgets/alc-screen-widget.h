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

#ifndef ALCSCREENWIDGET_H
#define ALCSCREENWIDGET_H

#include <QMainWindow>
#include <QPushButton>

class ALCEmitter;

namespace Ui { class ALCScreenWidget; }

class ALCScreenWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCScreenWidget *ui;

public:
  explicit ALCScreenWidget(QWidget *parent = 0);
  ~ALCScreenWidget();
  void setup();

private:
  void insertScreenCaptureItem(ALCEmitter *);
  void configure();

//  void prepareColorButton(QPushButton *, QColor);

};

#endif // ALCSCREENWIDGET_H
