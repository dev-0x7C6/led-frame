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

using unregister_callback = std::function<void()>;

namespace Functional {
class DevicePort;
class FramePaceSync;
} // namespace Functional

namespace Receiver {
namespace Concrete {

class UartWorker;

class UartReceiver final : public Receiver::Abstract::AbstractReceiver {
public:
	explicit UartReceiver(i32, std::unique_ptr<Functional::DevicePort> &&, unregister_callback &&unregister);
	UartReceiver(const UartReceiver &) = delete;
	UartReceiver(UartReceiver &&) = delete;
	~UartReceiver() final;

	auto type() const noexcept -> ReceiverType final;

	Container::DeviceConfigContainer config() final;

protected:
	void run(const std::atomic_bool &interrupted);

private:
	std::unique_ptr<Functional::DevicePort> m_device;
	QSerialPortInfo m_details;

	raii_thread m_thread;
	std::function<void()> m_unregister;
};
} // namespace Concrete
} // namespace Receiver
