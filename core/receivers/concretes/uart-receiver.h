#pragma once

#include <core/enums/color-format-enum.h>
#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/protocols/concretes/led-frame-protocol.h>

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <atomic>
#include <memory>
#include <initializer_list>

#include <externals/common/std/raii/raii-thread.hpp>

using unregister_callback = std::function<void(const IRepresentable &)>;

namespace Functional {
class DevicePort;
class FramePaceSync;
} // namespace Functional

namespace Receiver {
namespace Concrete {

class UartWorker;

class UartReceiver final : public Receiver::Abstract::AbstractReceiver {
public:
	explicit UartReceiver(Protocol::Concrete::LedFrameProtocol &&protocol, unregister_callback &&unregister);
	UartReceiver(const UartReceiver &) = delete;
	UartReceiver(UartReceiver &&) = delete;
	~UartReceiver() final;

	auto type() const noexcept -> receiver_type final;

protected:
	void run(const std::atomic_bool &interrupted);

private:
	Protocol::Concrete::LedFrameProtocol m_protocol;
	QSerialPortInfo m_details;

	raii_thread m_thread;
	std::function<void()> m_unregister;
	bool m_evenFrame{true};
};
} // namespace Concrete
} // namespace Receiver
