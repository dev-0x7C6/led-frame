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

#ifndef ALCEMITTERWIDGET_H
#define ALCEMITTERWIDGET_H

#include <QMainWindow>
#include <QPushButton>

class ALCEmitter;

class QPushButtonEx :public QPushButton {
private:
  ALCEmitter *m_emitter;

public:
  explicit QPushButtonEx(QWidget *parent = 0) :
    QPushButton(parent),
    m_emitter(0) {}

  void setEmitter(ALCEmitter *emitter) {
    m_emitter = emitter;
  }

  ALCEmitter *emitter() {
    return m_emitter;
  }
};


class QTreeWidgetItem;
class QBoxLayout;

namespace Ui { class ALCEmitterWidget; }

class ALCEmitterWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCEmitterWidget *ui;

public:
  explicit ALCEmitterWidget(QWidget *parent = 0);
  ~ALCEmitterWidget();

  void addPlainColorItem();
  void addAnimationItem();
  void addImageItem();

  void prepare();
  void setup();

private:
  void insertPlainColorItem(ALCEmitter *);
  void insertAnimationItem(ALCEmitter *);
  void insertImageItem(ALCEmitter *);

  void insertDefaultButtons(ALCEmitter *, QBoxLayout*);

  void reconfigure();
  void rename();
  void remove();


  void prepareColorItem(QTreeWidgetItem *, QColor);


};

#endif // ALCEMITTERSWIDGET_H
