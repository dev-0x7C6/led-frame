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

#include "alc-screen-widget.h"
#include "ui_alc-screen-widget.h"

#include "classes/alc-color-samples.h"
#include "emitters/alc-screen-emitter.h"
#include "managers/alc-emitter-manager.h"

#include "widgets/alc-emitter-widget.h"

ALCScreenWidget::ALCScreenWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCScreenWidget) {

  QPalette p = palette();
  p.setColor( QPalette::AlternateBase, QColor(226, 237, 253) );
  setPalette(p);

  ui->setupUi(this);
  setup();
  ui->tree->setAlternatingRowColors(true);
  ui->tree->setColumnCount(4);
  ui->tree->header()->setStretchLastSection(false);
  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
  ui->tree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
}

ALCScreenWidget::~ALCScreenWidget() {
  delete ui;
}

//#include <QListIterator>

#include <QDebug>

void ALCScreenWidget::setup() {
  ui->tree->clear();
  QListIterator < ALCEmitter*> ii(ALCEmitterManager::instance()->allEmitters());

  ALCEmitter *emitter;
  while (ii.hasNext()) {

    switch ((emitter = ii.next())->type()) {
    case ALCEmitter::EMITTER_SCREEN_CAPTURE:
      insertScreenCaptureItem(emitter);
      break;
    default:
      break;
    }
  }
}

void ALCScreenWidget::insertScreenCaptureItem(ALCEmitter *ptr) {
  ALCScreenEmitter *emitter = dynamic_cast < ALCScreenEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/screen.png"));


  QPushButtonEx *button = new QPushButtonEx();
  button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  button->setEmitter(emitter);
  button->setIcon(QIcon(":/16x16/configure.png"));
  button->setFlat(true);
  button->setToolTip(emitter->emitterName());
  connect(button, &QPushButtonEx::clicked, this, &ALCScreenWidget::configure);

  ui->tree->setItemWidget(item, 2, button);

  button = new QPushButtonEx();
  button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  button->setEmitter(emitter);
  button->setIcon(QIcon(":/16x16/rename.png"));
  button->setToolTip(emitter->emitterName());
  button->setFlat(true);
  button->setEnabled(false);
  connect(button, &QPushButtonEx::clicked, this, &ALCScreenWidget::configure);
  ui->tree->setItemWidget(item, 3, button);

}

#include "dialogs/alc-screen-configure-dialog.h"

#include "emitters/alc-screen-emitter.h"

void ALCScreenWidget::configure() {
  ALCScreenConfigureDialog dialog;
  dialog.setWindowTitle(reinterpret_cast< QPushButtonEx*>(sender())->toolTip());
  dialog.setEmitter(dynamic_cast< ALCScreenEmitter*> (reinterpret_cast< QPushButtonEx*>(sender())->emitter()));
  dialog.exec();
}

//void ALCScreenWidget::insertScreenCaptureItem(ALCEmitter *)
//{

//}

#include "emitters/alc-color-emitter.h"
#include "emitters/alc-animation-emitter.h"
#include "emitters/alc-image-emitter.h"

#include <QPushButton>

//void ALCScreenWidget::insertPlainColorItem(ALCEmitter *ptr) {
//  ALCALCEmitter *emitter = dynamic_cast < ALCALCEmitter*> ( ptr);
//  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
//  item->setText(0, emitter->emitterName());
//  emitter->setTreeItem(item);
//  item->setIcon(0, QIcon(":/22x22/color.png"));
//  QPushButtonEx *button = new QPushButtonEx();
//  button->setEmitter(emitter);
//  prepareColorButton(button, emitter->color());
//  ui->tree->setItemWidget(item, 1, button);
//  connect(button, &QPushButtonEx::clicked, this, &ALCScreenWidget::pickColor);
//}


