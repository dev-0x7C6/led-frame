#pragma once

#include <core/emitters/interfaces/iemitter.h>
#include <core/emitters/interfaces/iemitter-manager.h>

namespace Emitter {
namespace Abstract {

class AbstractEmitterManager : public Emitter::Interface::IEmitterManager {
public:
	explicit AbstractEmitterManager() = default;
	virtual ~AbstractEmitterManager() = default;

	virtual void attach(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detach(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	virtual void attach(Emitter::Interface::IEmitterNotify *notify) override;
	virtual void detach(Emitter::Interface::IEmitterNotify *notify) override;

	virtual std::shared_ptr<Interface::IEmitter> find(const int id) const override;
	virtual const std::list<std::shared_ptr<Interface::IEmitter>> &list() const override;

private:
	std::list<std::shared_ptr<Emitter::Interface::IEmitter>> m_emitters;
	std::list<Emitter::Interface::IEmitterNotify *> m_notifiers;
};
}
}
