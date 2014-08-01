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

#include "alc-color-correction-widget.h"
#include "classes/alc-color-correction.h"
#include "ui_alc-color-correction-widget.h"

ALCColorCorrectionWidget::ALCColorCorrectionWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ALCColorCorrectionWidget),
  m_correction(ALCColorCorrection::instance()) {
  ui->setupUi(this);
  connect(ui->brightnessSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::brightnessSliderChanged);
  connect(ui->blueSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::blueSliderChanged);
  connect(ui->greenSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::greenSliderChanged);
  connect(ui->redSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::redSliderChanged);
  connect(ui->restoreBrightness, &QPushButton::clicked, this, &ALCColorCorrectionWidget::restoreBrightness);
  connect(ui->restoreRed, &QPushButton::clicked, this, &ALCColorCorrectionWidget::restoreRed);
  connect(ui->restoreGreen, &QPushButton::clicked, this, &ALCColorCorrectionWidget::restoreGreen);
  connect(ui->restoreBlue, &QPushButton::clicked, this, &ALCColorCorrectionWidget::restoreBlue);
  reload();
}

ALCColorCorrectionWidget::~ALCColorCorrectionWidget() {
  delete ui;
}

void ALCColorCorrectionWidget::setColorCorrection(ALCColorCorrection *correction) {
  m_correction = correction;
  reload();
}

void ALCColorCorrectionWidget::restore() {
  ui->brightnessSlider->setValue(100.0);
  ui->blueSlider->setValue(100.0);
  ui->greenSlider->setValue(100.0);
  ui->redSlider->setValue(100.0);
  m_correction->setBrightness(1);
  m_correction->setRedCorrection(1);
  m_correction->setGreenCorrection(1);
  m_correction->setBlueCorrection(1);
}

void ALCColorCorrectionWidget::reload() {
  ui->brightnessSlider->setValue(m_correction->brightness() * 100.0);
  ui->blueSlider->setValue(m_correction->blueCorrection() * 100.0);
  ui->greenSlider->setValue(m_correction->greenCorrection() * 100.0);
  ui->redSlider->setValue(m_correction->redCorrection() * 100.0);
}

void ALCColorCorrectionWidget::brightnessSliderChanged(const int value) {
  m_correction->setBrightness(value / 100.0);
  ui->brightnessProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::blueSliderChanged(const int value) {
  m_correction->setBlueCorrection(value / 100.0);
  ui->blueProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::greenSliderChanged(const int value) {
  m_correction->setGreenCorrection(value / 100.0);
  ui->greenProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::redSliderChanged(const int value) {
  m_correction->setRedCorrection(value / 100.0);
  ui->redProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::restoreBrightness() {
  ui->brightnessSlider->setValue(100);
}

void ALCColorCorrectionWidget::restoreRed() {
  ui->redSlider->setValue(100);
}

void ALCColorCorrectionWidget::restoreGreen() {
  ui->greenSlider->setValue(100);
}

void ALCColorCorrectionWidget::restoreBlue() {
  ui->blueSlider->setValue(100);
}
