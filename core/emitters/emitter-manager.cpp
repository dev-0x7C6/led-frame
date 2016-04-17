#include <core/emitters/emitter-manager.h>
#include <QSettings>

using namespace Emitter::Manager;

EmitterManager::EmitterManager(QSettings &settings)
	: m_settings(settings)
	, m_firstRun(false)

{
}

void EmitterManager::load() {
	m_settings.beginGroup("emitters");
	m_firstRun = m_settings.value("isFirstRun", true).toBool();
	m_settings.endGroup();
}

void EmitterManager::save() {
	m_settings.beginGroup("emitters");
	m_settings.endGroup();
}

bool EmitterManager::isFirstRun() {
	return m_firstRun;
}
