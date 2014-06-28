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

#ifndef ALCSCREENCONFIGUREDIALOG_H
#define ALCSCREENCONFIGUREDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ALCScreenConfigureDialog;
}

class ALCScreenEmitter;

class ALCScreenConfigureDialog : public QDialog {
  Q_OBJECT
private:
  Ui::ALCScreenConfigureDialog *ui;
  ALCScreenEmitter *m_emitter;

public:
  explicit ALCScreenConfigureDialog(QWidget *parent = 0);
  ~ALCScreenConfigureDialog();

  void setEmitter(ALCScreenEmitter *);

private:
  void fpsValueChanged(int);
  void clipValueChanged(int);

  void accepted(QAbstractButton *);
};

#endif // ALCSCREENCONFIGUREDIALOG_H
