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

#include "alc-screen-configure-dialog.h"
#include "ui_alc-screen-configure-dialog.h"

ALCScreenConfigureDialog::ALCScreenConfigureDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ALCScreenConfigureDialog) {
  ui->setupUi(this);
  connect(ui->fps, &QSlider::valueChanged, this, &ALCScreenConfigureDialog::fpsValueChanged);
  connect(ui->clip, &QSlider::valueChanged, this, &ALCScreenConfigureDialog::clipValueChanged);
  connect(ui->buttons, &QDialogButtonBox::clicked, this, &ALCScreenConfigureDialog::accepted);
}

ALCScreenConfigureDialog::~ALCScreenConfigureDialog() {
  delete ui;
}

#include "emitters/alc-screen-emitter.h"


void ALCScreenConfigureDialog::setEmitter(ALCScreenEmitter *emitter) {
  m_emitter = emitter;

  ui->areaX->setValue(emitter->area().x());
  ui->areaY->setValue(emitter->area().y());
  ui->areaWidth->setValue(emitter->area().width());
  ui->areaHeight->setValue(emitter->area().height());

  ui->fps->setValue(emitter->framerateLimit());
  ui->clip->setValue(emitter->marginProcent() * 100.0);
  fpsValueChanged(emitter->framerateLimit());
  clipValueChanged(emitter->marginProcent() * 100.0);


  ui->sampleSize->clear();

  int px;

  QIcon *icon[3];
  icon[0] = new QIcon(":/custom/green.png");
  icon[1] = new QIcon(":/custom/blue.png");
  icon[2] = new QIcon(":/custom/red.png");

  ui->sampleSize->clear();

  for (register int i = 0; i < 12; ++i) {
    QIcon *qi;
    px = 32 + (32 * i);
    if (px <= 160)
      qi = icon[0]; else
    if (px <= 256)
      qi = icon[1]; else
      qi = icon[2];
    ui->sampleSize->addItem(*qi, QString("Pixel sample: %1 px").arg(QString::number(px)), QVariant::fromValue(px));
  }

  ui->sampleSize->setCurrentIndex(emitter->chunk() / 32 - 1);

  ui->pixelSkip->clear();

  for (register int i = 0; i < 9; ++i) {
    px = 4 * i;
    QIcon *qi;
    if (px <= 4)
      qi = icon[2]; else
    if (px <= 8)
      qi = icon[1]; else
      qi = icon[0];
    ui->pixelSkip->addItem(*qi, QString("Pixel sample: %1 px").arg(QString::number(px)), QVariant::fromValue(px));
  }

  ui->pixelSkip->setCurrentIndex(emitter->pixelSkip() / 4);

  ui->correction->setColorCorrection(emitter);

}

void ALCScreenConfigureDialog::fpsValueChanged(int value) {
  ui->fpsLabel->setText(QString("%1 fps").arg(value));
  if (value > 60)
    ui->fpsCpu->setPixmap(QPixmap(":/custom/red.png")); else
  if (value > 25)
    ui->fpsCpu->setPixmap(QPixmap(":/custom/blue.png")); else
    ui->fpsCpu->setPixmap(QPixmap(":/custom/green.png"));
}

void ALCScreenConfigureDialog::clipValueChanged(int value) {
  ui->clipLabel->setText(QString("%1%").arg(QString::number(value)));
}

#include <QScreen>
#include <QDesktopWidget>

void ALCScreenConfigureDialog::accepted(QAbstractButton *button) {
  if (!m_emitter)
    return;

  QRect area;
  QString str;
  QList < QScreen* > screens = QApplication::screens();

  switch (ui->buttons->buttonRole(button)) {
  case QDialogButtonBox::ApplyRole:
    area.setX(ui->areaX->value());
    area.setY(ui->areaY->value());
    area.setWidth(ui->areaWidth->value());
    area.setHeight(ui->areaHeight->value());
    m_emitter->setFramerateLimit(ui->fps->value());
    m_emitter->setCaptureArea(area);
    m_emitter->setMarginProcent(ui->clip->value() / 100.0);
    m_emitter->setChunkSize(ui->sampleSize->currentData().toInt());
    m_emitter->setPixelSkip(ui->pixelSkip->currentData().toInt());
    close();
    break;
  case QDialogButtonBox::ResetRole:
    str = m_emitter->name();
    for (register int i = 0; i < screens.count(); ++i)
      if (screens[i]->name() == str) {
        area = screens[i]->geometry();
        break;
      }

    m_emitter->setCaptureArea(area);
    m_emitter->setFramerateLimit(24);
    m_emitter->setMarginProcent(0.02);
    m_emitter->setChunkSize(160);
    m_emitter->setPixelSkip(16);
    ui->correction->restore();
    setEmitter(m_emitter);
    break;

  default:
    close();
    break;
  }
}
