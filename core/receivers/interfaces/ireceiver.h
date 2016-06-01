#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/device-config-container.h>
#include <core/enums/receiver-type-enum.h>

#include <QString>

namespace Emitter {
namespace Interface {
class IEmitter;
}
}

namespace Corrector {
namespace Concrete {
namespace Manager {
class CorrectorManager;
}
}
}

namespace Receiver {
namespace Interface {

class IReceiver {
public:
	explicit IReceiver() = default;
	virtual ~IReceiver() = default;

	virtual QString name() const = 0;
	virtual Enum::ReceiverType type() const = 0;

	virtual void connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) = 0;
	virtual bool isEmitterConnected() = 0;

	virtual const std::shared_ptr<Emitter::Interface::IEmitter> &connectedEmitter() const = 0;

	virtual Container::DeviceConfigContainer config() = 0;
	virtual void setName(const QString &name) = 0;

	virtual void changed(const std::function<void()> &callback) = 0;

	virtual Corrector::Concrete::Manager::CorrectorManager *correctorManager() = 0;
};
}
}
