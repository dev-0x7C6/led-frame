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

ALCColorCorrection *ALCColorCorrectionWidget::colorCorrection() {
  return m_correction;
}

void ALCColorCorrectionWidget::restore() {
  ui->brightnessSlider->setValue(100.0);
  ui->blueSlider->setValue(100.0);
  ui->greenSlider->setValue(100.0);
  ui->redSlider->setValue(100.0);
  m_correction->setCorrection(ALCColorCorrection::Brightness, 1.0);
  m_correction->setCorrection(ALCColorCorrection::Red, 1.0);
  m_correction->setCorrection(ALCColorCorrection::Green, 1.0);
  m_correction->setCorrection(ALCColorCorrection::Blue, 1.0);
}

void ALCColorCorrectionWidget::reload() {
  ui->brightnessSlider->setValue(m_correction->correction(ALCColorCorrection::Brightness) * 100.0);
  ui->blueSlider->setValue(m_correction->correction(ALCColorCorrection::Blue) * 100.0);
  ui->greenSlider->setValue(m_correction->correction(ALCColorCorrection::Green) * 100.0);
  ui->redSlider->setValue(m_correction->correction(ALCColorCorrection::Red) * 100.0);
}

void ALCColorCorrectionWidget::brightnessSliderChanged(const int value) {
  m_correction->setCorrection(ALCColorCorrection::Brightness, value / 100.0);
  ui->brightnessProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::blueSliderChanged(const int value) {
  m_correction->setCorrection(ALCColorCorrection::Blue, value / 100.0);
  ui->blueProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::greenSliderChanged(const int value) {
  m_correction->setCorrection(ALCColorCorrection::Green, value / 100.0);
  ui->greenProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::redSliderChanged(const int value) {
  m_correction->setCorrection(ALCColorCorrection::Red, value / 100.0);
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
