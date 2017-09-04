#pragma once

#include <core/interfaces/icorrector.h>
#include <core/generic/atom-aggregator.h>

#include <core/interfaces/inotification-aggregator.h>

#include <memory>
#include <list>

#include <QTimer>

namespace Functional {
class DevicePort;
}

namespace Network {
class UdpBroadcastService;
}

class IReceiver;
class QSettings;

namespace Manager {

class MainManager final : public QObject, public INotificationAggregator {
	Q_OBJECT
public:
	explicit MainManager(QSettings &settings);
	~MainManager() override;

	virtual void attach(INotification &notifier) noexcept override final;
	virtual void detach(INotification &notifier) noexcept override final;

	auto &atoms() noexcept { return m_atoms; }

	void setRegisterDeviceCallback(const std::function<bool(IReceiver *, const QString &serialNumber)> &callback);
	void run();

private:
	QSettings &m_settings;

private:
	std::shared_ptr<ICorrector> m_globalBrightnessCorrection;
	std::shared_ptr<ICorrector> m_globalRedCorrection;
	std::shared_ptr<ICorrector> m_globalGreenCorrection;
	std::shared_ptr<ICorrector> m_globalBlueCorrection;

private:
	AtomAggregator m_atoms;

protected:
	void rescan();

private:
	std::function<bool(IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;
	std::list<std::unique_ptr<Network::UdpBroadcastService>> m_broadcasts;
	QTimer m_deviceScan;
};
}
