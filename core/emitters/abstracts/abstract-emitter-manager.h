#pragma once

#include <core/emitters/interfaces/iemitter.h>
#include <core/emitters/interfaces/iemitter-manager.h>

namespace Emitter {
namespace Abstract {

class AbstractEmitterManager : public Emitter::Interface::IEmitterManager {
public:
	explicit AbstractEmitterManager() = default;
	~AbstractEmitterManager() override = default;

	void attach(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void detach(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	void attach(Emitter::Interface::IEmitterNotify *notify) override;
	void detach(Emitter::Interface::IEmitterNotify *notify) override;

	std::shared_ptr<Interface::IEmitter> find(const int id) const override;
	const std::list<std::shared_ptr<Interface::IEmitter>> &list() const override;

private:
	std::list<std::shared_ptr<Emitter::Interface::IEmitter>> m_emitters;
	std::list<Emitter::Interface::IEmitterNotify *> m_notifiers;
};
}
}
