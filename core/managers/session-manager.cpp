#include "session-manager.h"

using namespace Manager;

SessionManager::SessionManager(QSettings &settings, MainManager &mainManager)
		: m_settings(settings)
		, m_mainManager(mainManager) {
	static_cast<void>(m_settings);
	static_cast<void>(m_mainManager);
}
