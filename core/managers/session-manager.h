#pragma once

#include <QTimer>
#include <memory>

class IReceiver;
class QSettings;
class QString;
class MultiCameraController;

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
	QTimer m_invalidateTimer;
	QSettings &m_settings;
	MainManager &m_mainManager;
	std::unique_ptr<MultiCameraController> m_cameraManager;
};
} // namespace Manager
