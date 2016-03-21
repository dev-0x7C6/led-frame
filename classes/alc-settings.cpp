#include "alc-settings.h"

#include <QSettings>

ALCSettings::ALCSettings() :
	m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver")) {
	load();
}

ALCSettings::~ALCSettings() {
	save();
	delete m_settings;
}

QSettings *ALCSettings::settings() {
	return m_settings;
}

void ALCSettings::load() {
	//  m_settings->beginGroup("GlobalColorCorrection");
	//  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Brightness, m_settings->value("Brightness", 1.0).toDouble());
	//  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Blue, m_settings->value("Blue", 1.0).toDouble());
	//  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Green, m_settings->value("Green", 1.0).toDouble());
	//  Correctors::ALCColorCorrection::instance()->setCorrection(Correctors::ALCColorCorrection::Color::Red, m_settings->value("Red", 1.0).toDouble());
	//  m_settings->endGroup();
}

void ALCSettings::save() {
	//  m_settings->beginGroup("GlobalColorCorrection");
	//  m_settings->setValue("Brightness", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Brightness));
	//  m_settings->setValue("Blue", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Blue));
	//  m_settings->setValue("Green", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Green));
	//  m_settings->setValue("Red", Correctors::ALCColorCorrection::instance()->correction(Correctors::ALCColorCorrection::Color::Red));
	//  m_settings->endGroup();
}
