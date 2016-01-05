#pragma once

#include <core/interfaces/templates/itemplate-attach.h>
#include <core/containers/color-scanline-container.h>
#include <core/enums/receiver-type-enum.h>
#include <core/interfaces/icolor-corrector.h>

#include <QString>

namespace Interface {

	class IEmitter;

	class IReceiver {
	public:
		explicit IReceiver() = default;
		virtual ~IReceiver() = default;

		virtual QString name() const = 0;
		virtual Enum::ReceiverType type() const = 0;

		virtual void connectEmitter(std::shared_ptr<IEmitter> &emitter) = 0;

	};

}
