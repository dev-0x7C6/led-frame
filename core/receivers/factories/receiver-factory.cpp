#include <core/receivers/factories/receiver-factory.h>
#include <core/receivers/receiver-type.hpp>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/receivers/stub-receiver.h>
#include <core/devices/device-port.h>

#include <core/protocols/concretes/led-frame-protocol.h>

std::unique_ptr<IReceiver> Factory::make_receiver(receiver_type type, std::function<void(const IRepresentable &)> &&unregister, std::any &&args) {
	using namespace Receiver::Concrete;
	switch (type) {
		case receiver_type::uart: {
			const auto info = std::any_cast<QSerialPortInfo>(args);

			try {
				Protocol::Concrete::LedFrameProtocol protocol(info);
				return std::make_unique<UartReceiver>(std::move(protocol), std::move(unregister));
			} catch (std::logic_error &) {
				return nullptr;
			}
		}
		case receiver_type::stub:
			return std::make_unique<StubReceiver>();
	}

	return nullptr;
}
