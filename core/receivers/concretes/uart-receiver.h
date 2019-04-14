#pragma once

#include <core/containers/device-config-container.h>
#include <core/enums/color-format-enum.h>
#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/containers/led-ribbon-config-container.h>

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <atomic>
#include <memory>
#include <initializer_list>

#include <externals/common/std/raii/raii-thread.hpp>

namespace Functional {
class DevicePort;
class LoopSync;
} // namespace Functional

namespace Receiver {
namespace Concrete {

class UartWorker;

class UartReceiver final : public QObject, public Receiver::Abstract::AbstractReceiver {
	Q_OBJECT
public:
	explicit UartReceiver(i32, std::unique_ptr<Functional::DevicePort> &&);
	~UartReceiver() final;

	auto type() const noexcept -> ReceiverType final;

	Container::DeviceConfigContainer config() final;

protected:
	void run(const std::atomic_bool &interrupted);

private:
	std::unique_ptr<Functional::DevicePort> m_device;
	QSerialPortInfo m_details;

	raii_thread m_thread;

signals:
	void finished();
};
} // namespace Concrete
} // namespace Receiver
