#pragma once

#include <core/interfaces/iemitter.h>
#include <core/interfaces/iemitter-manager.h>

namespace Abstract {

	class AbstractEmitterManager : public Interface::IEmitterManager {
	public:
		explicit AbstractEmitterManager() = default;
		virtual ~AbstractEmitterManager() = default;

		virtual void attach(std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual void detach(std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual void attach(Interface::IEmitterNotify *notify) override;
		virtual void detach(Interface::IEmitterNotify *notify) override;

		virtual void populate() override;

		virtual const std::list<std::shared_ptr<Interface::IEmitter>> &list() const override;

	private:
		std::list<std::shared_ptr<Interface::IEmitter>> m_emitters;
		std::list<Interface::IEmitterNotify *> m_notifiers;

	};

}
