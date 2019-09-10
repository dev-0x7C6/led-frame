#include <core/receivers/factories/receiver-factory.h>
#include <core/receivers/receiver-type.hpp>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/receivers/stub-receiver.h>
#include <core/devices/device-port.h>

std::unique_ptr<IReceiver> factory::make_receiver(receiver_type type, std::function<void(const IRepresentable &)> &&unregister, std::any &&args) {
	using namespace Receiver::Concrete;
	switch (type) {
		case receiver_type::uart:
			return std::make_unique<UartReceiver>(std::make_unique<Functional::DevicePort>(std::any_cast<QSerialPortInfo>(args)), std::move(unregister));
		case receiver_type::stub:
			return std::make_unique<StubReceiver>();
	}

	return nullptr;
}
