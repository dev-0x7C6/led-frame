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
				auto info = std::any_cast<QSerialPortInfo>(args);
				auto name = std::string(make_porotocol(info).info().name);
				auto receiver = std::make_unique<UartReceiver>(std::move(info), std::move(unregister));
				receiver->setName(name);
				return receiver;
			} catch (std::logic_error &) {
				return nullptr;
			}
		case receiver_type::stub:
			return std::make_unique<StubReceiver>();
	}

	return nullptr;
}
