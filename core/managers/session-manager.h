#pragma once

class IReceiver;
class QSettings;
class QString;

namespace Manager {

class MainManager;

class SessionManager {
public:
	explicit SessionManager(QSettings &settings, MainManager &mainManager);
	virtual ~SessionManager();

protected:
	bool registerDevice(IReceiver *receiver, const QString &serialNumber);
	void createCorrectorGroup(IReceiver *receiver);

protected:
	void load();
	void save();

private:
	QSettings &m_settings;
	MainManager &m_mainManager;
};
} // namespace Manager
