#pragma once

#include <core/interfaces/iemitter-manager.h>
#include <core/interfaces/iemitter.h>

namespace Abstract {

	class AbstractEmitterManager : public Interface::IEmitterManager {
	public:
		explicit AbstractEmitterManager() = default;
		virtual ~AbstractEmitterManager() = default;

		virtual void attach(std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual void detach(std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual const std::list<std::shared_ptr<Interface::IEmitter>> &list() const override;

	private:
		std::list<std::shared_ptr<Interface::IEmitter>> m_emitters;
	};

}
