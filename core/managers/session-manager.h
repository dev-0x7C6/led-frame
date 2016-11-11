#pragma once

class QSettings;

namespace Manager {

class MainManager;

class SessionManager {
public:
	explicit SessionManager(QSettings &settings, MainManager &mainManager);

private:
	QSettings &m_settings;
	MainManager &m_mainManager;
};
}
