#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/device-config-container.h>
#include <core/enums/receiver-type-enum.h>

#include <QString>

#include <memory>
#include <core/generic/iatom.h>
#include <core/generic/atom-aggregator.h>

class IEmitter;

namespace Corrector {
namespace Concrete {
class CorrectorManager;
}
}

class IReceiver : public IAtom {
public:
	explicit IReceiver(ci32 id)
			: IAtom(id) {}
	~IReceiver() override = default;

	virtual auto category() const noexcept -> Category final { return Category::Receiver; }

	virtual QString name() const = 0;
	virtual Enum::ReceiverType type() const = 0;

	virtual u32 framerate() const { return 90; }

	virtual void disconnectEmitter() = 0;
	virtual void connectEmitter(const std::shared_ptr<IEmitter> &emitter) = 0;
	virtual bool isEmitterConnected() const = 0;
	virtual int connectedEmitterId() const = 0;

	virtual std::shared_ptr<IEmitter> connectedEmitter() const = 0;

	virtual Container::DeviceConfigContainer config() = 0;
	virtual void setName(const QString &name) = 0;

	virtual auto correctors() noexcept -> AtomAggregator & = 0;
};
