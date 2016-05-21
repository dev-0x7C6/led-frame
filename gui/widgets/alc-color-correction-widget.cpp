#include "alc-color-correction-widget.h"
#include "ui_alc-color-correction-widget.h"

ALCColorCorrectionWidget::ALCColorCorrectionWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::ALCColorCorrectionWidget) {
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

void ALCColorCorrectionWidget::setColorCorrection(Correctors::ALCColorCorrection *correction) {
	m_correction = correction;
	reload();
}

Correctors::ALCColorCorrection *ALCColorCorrectionWidget::colorCorrection() {
	return m_correction;
}

void ALCColorCorrectionWidget::restore() {
	ui->brightnessSlider->setValue(100.0);
	ui->blueSlider->setValue(100.0);
	ui->greenSlider->setValue(100.0);
	ui->redSlider->setValue(100.0);
}

void ALCColorCorrectionWidget::reload() {
	//  double brightness = m_correction->correction(Correctors::ALCColorCorrection::Color::Brightness);
	//  ui->brightnessSlider->setValue(brightness * 100.0);
	//  ui->blueSlider->setValue(m_correction->correction(Correctors::ALCColorCorrection::Color::Blue) * 100.0);
	//  ui->greenSlider->setValue(m_correction->correction(Correctors::ALCColorCorrection::Color::Green) * 100.0);
	//  ui->redSlider->setValue(m_correction->correction(Correctors::ALCColorCorrection::Color::Red) * 100.0);
	//  emit brightnessChanged(brightness);
}

void ALCColorCorrectionWidget::brightnessSliderChanged(const int) {
	//  m_correction->setCorrection(Correctors::ALCColorCorrection::Color::Brightness, value / 100.0);
	//  ui->brightnessProcent->setText(QString::number(value) + "%");
	//  emit brightnessChanged(value / 100.0);
}

void ALCColorCorrectionWidget::blueSliderChanged(const int) {
	//  m_correction->setCorrection(Correctors::ALCColorCorrection::Color::Blue, value / 100.0);
	//  ui->blueProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::greenSliderChanged(const int) {
	//  m_correction->setCorrection(Correctors::ALCColorCorrection::Color::Green, value / 100.0);
	//  ui->greenProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::redSliderChanged(const int) {
	//  m_correction->setCorrection(Correctors::ALCColorCorrection::Color::Red, value / 100.0);
	//  ui->redProcent->setText(QString::number(value) + "%");
}

void ALCColorCorrectionWidget::restoreBrightness() {
	ui->brightnessSlider->setValue(100);
	emit brightnessChanged(1.0);
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
