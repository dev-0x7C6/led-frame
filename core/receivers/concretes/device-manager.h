#pragma once

#include <functional>
#include <list>
#include <memory>

#include <QObject>
#include <QTimer>

#include <core/receivers/abstracts/abstract-receiver-manager.h>

namespace Functional {
class DevicePort;
}

namespace Network {
	class BroadcastService;
}

namespace Receiver {
namespace Concrete {

class UartReceiver;

class ReceiverManager : public Receiver::Abstract::AbstractReceiverManager {
	Q_OBJECT
public:
	explicit ReceiverManager();
	~ReceiverManager() override;

	void setRegisterDeviceCallback(const std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> &callback);

	void run();

protected:
	void rescan();

private:
	std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;
	std::list<std::unique_ptr<Network::BroadcastService>> m_broadcasts;
	QTimer m_deviceScan;

signals:
	void afterAttach();
};
}
}
