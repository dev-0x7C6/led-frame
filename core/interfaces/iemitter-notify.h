#pragma once

#include <memory>

namespace Interface {

	class IEmitter;

	class IEmitterNotify {
	public:
		explicit IEmitterNotify() = default;
		virtual ~IEmitterNotify() = default;

		virtual void attached(const std::shared_ptr<IEmitter> &emitter) = 0;
		virtual void detached(const std::shared_ptr<IEmitter> &emitter) = 0;

	};

}
