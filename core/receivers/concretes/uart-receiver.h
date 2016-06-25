#pragma once

#include <core/containers/device-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/functionals/lambda-thread.h>
#include <core/receivers/abstracts/abstract-receiver.h>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <atomic>
#include <memory>

namespace Functional {
class DevicePort;
}

namespace Receiver {
namespace Concrete {

class UartReceiver : public Receiver::Abstract::AbstractReceiver {
	Q_OBJECT
public:
	explicit UartReceiver(std::unique_ptr<Functional::DevicePort> &&device, QSerialPortInfo details);
	virtual ~UartReceiver();

	virtual Enum::ReceiverType type() const override;

	QSerialPortInfo details();
	void interrupt();

	virtual Container::DeviceConfigContainer config() override;

protected:
	void run();

private:
	std::unique_ptr<Functional::DevicePort> m_device;
	QSerialPortInfo m_details;
	std::atomic<bool> m_interrupt;
	Functional::LambdaThread m_thread;

signals:
	void finished();
};
}
}
