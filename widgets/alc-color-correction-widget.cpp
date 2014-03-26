#include "alc-color-correction-widget.h"
#include "ui_alc-color-correction-widget.h"

#include "classes/alc-color-correction.h"

ALCColorCorrectionWidget::ALCColorCorrectionWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ALCColorCorrectionWidget)
{
  ui->setupUi(this);

  connect(ui->brightnessSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::brightnessSliderChanged);
  connect(ui->blueSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::blueSliderChanged);
  connect(ui->greenSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::greenSliderChanged);
  connect(ui->redSlider, &QSlider::valueChanged, this, &ALCColorCorrectionWidget::redSliderChanged);

  connect(ui->restoreBrightness, &QPushButton::clicked, this, &ALCColorCorrectionWidget::setDefaultBrightness);
  connect(ui->restoreRed, &QPushButton::clicked, this, &ALCColorCorrectionWidget::setDefaultRed);
  connect(ui->restoreGreen, &QPushButton::clicked, this, &ALCColorCorrectionWidget::setDefaultGreen);
  connect(ui->restoreBlue, &QPushButton::clicked, this, &ALCColorCorrectionWidget::setDefaultBlue);

  fromGlobal();
}

ALCColorCorrectionWidget::~ALCColorCorrectionWidget() {
  delete ui;
}

void ALCColorCorrectionWidget::fromGlobal() {
  ui->brightnessSlider->setValue(ALCColorCorrection::instance()->brightness() * 100.0d);
  ui->blueSlider->setValue(ALCColorCorrection::instance()->blueCorrection() * 100.0d);
  ui->greenSlider->setValue(ALCColorCorrection::instance()->greenCorrection() * 100.0d);
  ui->redSlider->setValue(ALCColorCorrection::instance()->redCorrection() * 100.0d);
}

void ALCColorCorrectionWidget::brightnessSliderChanged(int value) {
  register double brightness = value / 100.0d;
  ALCColorCorrection::instance()->setBrightness(brightness);
  ui->brightnessProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::blueSliderChanged(int value) {
  register double blue = value / 100.0d;
  ALCColorCorrection::instance()->setBlueCorrection(blue);
  ui->blueProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::greenSliderChanged(int value) {
  register double green = value / 100.0d;
  ALCColorCorrection::instance()->setGreenCorrection(green);
  ui->greenProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::redSliderChanged(int value) {
  register double red = value / 100.0d;
  ALCColorCorrection::instance()->setRedCorrection(red);
  ui->redProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::setDefaultBrightness() {
  ui->brightnessSlider->setValue(100);
}

void ALCColorCorrectionWidget::setDefaultRed() {
  ui->redSlider->setValue(100);
}

void ALCColorCorrectionWidget::setDefaultGreen() {
  ui->greenSlider->setValue(100);
}

void ALCColorCorrectionWidget::setDefaultBlue() {
  ui->blueSlider->setValue(100);
}
