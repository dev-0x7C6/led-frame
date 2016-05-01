#pragma once

#include <list>
#include <memory>

namespace Interface {

	class IEmitter;
	class IEmitterNotify;

	class IEmitterManager {
	public:
		explicit IEmitterManager() = default;
		virtual ~IEmitterManager() = default;

		virtual void attach(std::shared_ptr<IEmitter> &emitter) = 0;
		virtual void detach(std::shared_ptr<IEmitter> &emitter) = 0;

		virtual void attach(IEmitterNotify *notify) = 0;
		virtual void detach(IEmitterNotify *notify) = 0;

		virtual const std::list<std::shared_ptr<IEmitter>> &list() const = 0;

		virtual void load() = 0;
		virtual void save() = 0;

		virtual bool isFirstRun() = 0;

	};

}
