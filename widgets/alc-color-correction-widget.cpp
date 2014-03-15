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
