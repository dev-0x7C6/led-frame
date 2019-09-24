#include <core/receivers/factories/receiver-factory.h>
#include <core/receivers/receiver-type.hpp>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/receivers/stub-receiver.h>
#include <core/devices/device-port.h>

#include <core/protocols/concretes/led-frame-protocol.h>

namespace {
Protocol::Concrete::LedFrameProtocol make_porotocol(const QSerialPortInfo &info) {
	return {info};
}
} // namespace

std::unique_ptr<IReceiver> Factory::make_receiver(receiver_type type, std::function<void(const IRepresentable &)> &&unregister, std::any &&args) {
	using namespace Receiver::Concrete;
	switch (type) {
		case receiver_type::uart:
			try {
				return std::make_unique<UartReceiver>(make_porotocol(std::any_cast<QSerialPortInfo>(args)), std::move(unregister));
			} catch (std::logic_error &) {
				return nullptr;
			}
		case receiver_type::stub:
			return std::make_unique<StubReceiver>();
	}

	return nullptr;
}
