#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/containers/device-config-container.h>
#include <core/enums/receiver-type-enum.h>
#include <core/abstracts/abstract-corrector-attach.h>

#include <QString>

namespace Interface {

	class IEmitter;

	class IReceiver : public Abstract::AbstractCorrectorAttach {
	public:
		explicit IReceiver() = default;
		virtual ~IReceiver() = default;

		virtual QString name() const = 0;
		virtual Enum::ReceiverType type() const = 0;

		virtual void connectEmitter(const std::shared_ptr<IEmitter> &emitter) = 0;
		virtual bool isEmitterConnected() = 0;

		virtual const std::shared_ptr<IEmitter> &connectedEmitter() const = 0;

		virtual Container::DeviceConfigContainer config() = 0;
		virtual void setName(const QString &name) = 0;

		virtual void changed(const std::function<void()> &callback) = 0;

	};

}
