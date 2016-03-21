#include "alc-led-strip-manager.h"

#include "classes/alc-strip-configuration.h"
#include "classes/alc-settings.h"

#include <QSettings>

ALCLedStripManager::ALCLedStripManager() {
	QSettings *settings = ALCSettings::instance()->settings();

	if (settings->childGroups().contains("led-strips")) {
		settings->beginGroup("led-strips");
		settings->endGroup();
	} else {
		settings->beginGroup("led-strips");
		settings->endGroup();
	}
}

ALCLedStripManager::~ALCLedStripManager() {
}

const QList<ALCStripConfiguration *> &ALCLedStripManager::configurations() {
	return m_configurations;
}

ALCLedStripManager *ALCLedStripManager::instance() {
	static ALCLedStripManager object;
	return &object;
}
