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

namespace Receiver {
namespace Concrete {

class DeviceReceiver;

class ReceiverManager : public QObject, public Receiver::Abstract::AbstractReceiverManager {
	Q_OBJECT
public:
	explicit ReceiverManager(QObject *parent = nullptr);
	virtual ~ReceiverManager();

	void setRegisterDeviceCallback(const std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> &callback);

	void run();

protected:
	void rescan();

private:
	std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;
	QTimer m_deviceScan;

signals:
	void afterAttach();
};
}
}