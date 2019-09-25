#pragma once

#include <core/interfaces/icorrector.h>
#include <core/generic/atom-aggregator.h>

#include <core/interfaces/inotification-aggregator.h>

#include <core/functionals/safe-queue.h>
#include <core/functionals/safe-set.h>
#include <core/functionals/safe-device-locker.h>

#include <list>
#include <memory>

#include <QTimer>

namespace Functional {
class DevicePort;
}

namespace Network {
class UdpBroadcastService;
}

class IReceiver;
class QSettings;
class SystemSerialPortConfiguration;

namespace Manager {

class MainManager final : public QObject, public INotificationAggregator {
	Q_OBJECT
public:
	explicit MainManager(QSettings &settings);
	~MainManager() override;

	void attach(INotification &notifier) noexcept final;
	void detach(INotification &notifier) noexcept final;

	auto &atoms() noexcept { return m_atoms; }

	void setRegisterDeviceCallback(const std::function<bool(IReceiver &)> &callback);
	void run();

private:
	QSettings &m_settings;

private:
	std::unique_ptr<SystemSerialPortConfiguration> m_serialConfig;
	std::shared_ptr<ICorrector> m_globalBrightnessCorrection;
	std::shared_ptr<ICorrector> m_globalRedCorrection;
	std::shared_ptr<ICorrector> m_globalGreenCorrection;
	std::shared_ptr<ICorrector> m_globalBlueCorrection;

protected:
	void rescan();

private:
	std::function<bool(IReceiver &)> m_registerDeviceCallback;
	std::list<std::unique_ptr<Network::UdpBroadcastService>> m_broadcasts;
	QTimer m_deviceScan;

	safe::device_locker m_deviceLocker;
	safe::queue<i32> m_unregisterQueue;
	AtomAggregator m_atoms;
};
} // namespace Manager
