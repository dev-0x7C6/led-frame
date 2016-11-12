#pragma once

namespace Receiver {
namespace Interface {
class IReceiver;
}
}

class QString;
class QSettings;

namespace Manager {

class MainManager;

class SessionManager {
public:
	explicit SessionManager(QSettings &settings, MainManager &mainManager);
	virtual ~SessionManager();

protected:
	bool registerDevice(Receiver::Interface::IReceiver *receiver, const QString &serialNumber);
	void createCorrectorGroup(Receiver::Interface::IReceiver *receiver);

protected:
	void load();
	void save();

private:
	QSettings &m_settings;
	MainManager &m_mainManager;
};
}
