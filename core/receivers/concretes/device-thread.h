#pragma once

#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/containers/device-config-container.h>
#include <core/enums/color-format-enum.h>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <atomic>
#include <memory>

namespace Functional {
class DevicePort;
}

namespace Receiver {
namespace Concrete {

class DeviceReceiver : public QThread, public Receiver::Abstract::AbstractReceiver {
	Q_OBJECT
public:
	explicit DeviceReceiver(std::unique_ptr<Functional::DevicePort> &&device, QSerialPortInfo details, QObject *parent = 0);
	virtual ~DeviceReceiver();

	virtual Enum::ReceiverType type() const override;

	QSerialPortInfo details();
	void interrupt();

	virtual Container::DeviceConfigContainer config() override;

protected:
	virtual void run() override;

private:
	std::unique_ptr<Functional::DevicePort> m_device;
	QSerialPortInfo m_details;
	std::atomic<bool> m_interrupt;
};
}
}
