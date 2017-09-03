#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <core/interfaces/imulti-notifier.h>
#include <core/interfaces/imulti-notifier-manager.h>
#include <core/generic/atom-aggregator.h>

#include <memory>
#include <list>

#include <QTimer>

namespace Functional {
class DevicePort;
}

namespace Network {
class UdpBroadcastService;
}

class QSettings;

namespace Manager {

class MainManager final : public QObject, public Interface::IMutliNotifierManager {
	Q_OBJECT
public:
	explicit MainManager(QSettings &settings);
	~MainManager() override;

	void attach(Interface::IMultiNotifier &notifier) override;
	void detach(Interface::IMultiNotifier &notifier) override;

	auto &atoms() noexcept { return m_atoms; }

	void setRegisterDeviceCallback(const std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> &callback);
	void run();

private:
	QSettings &m_settings;

private:
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBrightnessCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalRedCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalGreenCorrection;
	std::shared_ptr<Corrector::Interface::ICorrector> m_globalBlueCorrection;

private:
	AtomAggregator m_atoms;

protected:
	void rescan();

private:
	std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;
	std::list<std::unique_ptr<Network::UdpBroadcastService>> m_broadcasts;
	QTimer m_deviceScan;
};
}
