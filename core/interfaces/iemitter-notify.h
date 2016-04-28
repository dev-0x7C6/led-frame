#pragma once

namespace Interface {

	class IEmitter;

	class IEmitterNotify {
	public:
		explicit IEmitterNotify() = default;
		virtual ~IEmitterNotify() = default;

		virtual void attached(IEmitter *emitter) = 0;
		virtual void detached(IEmitter *emitter) = 0;

	};

}
