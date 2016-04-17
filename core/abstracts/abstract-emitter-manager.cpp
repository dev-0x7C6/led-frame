#include <core/abstracts/abstract-emitter-manager.h>

using namespace Abstract;

void AbstractEmitterManager::attach(std::shared_ptr<Interface::IEmitter> &emitter) {
	m_emitters.push_back(emitter);
}

void AbstractEmitterManager::detach(std::shared_ptr<Interface::IEmitter> &emitter) {
	m_emitters.remove_if([&emitter](const auto & value)  {
		return (emitter.get() == value.get());
	});
}

const std::list<std::shared_ptr<Interface::IEmitter>> &AbstractEmitterManager::list() const {
	return m_emitters;
}
