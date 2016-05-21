#include <core/abstracts/abstract-emitter-manager.h>
#include <core/interfaces/iemitter-notify.h>

using namespace Abstract;
using namespace Interface;

void AbstractEmitterManager::attach(std::shared_ptr<IEmitter> &emitter) {
	m_emitters.push_back(emitter);

	for (const auto &notify : m_notifiers)
		notify->attached(emitter);
}

void AbstractEmitterManager::detach(std::shared_ptr<IEmitter> &emitter) {
	for (const auto &notify : m_notifiers)
		notify->detached(emitter);

	m_emitters.remove_if([&emitter](const auto &match) -> bool{
		return (emitter.get() == match.get());
	});
}

const std::list<std::shared_ptr<IEmitter>> &AbstractEmitterManager::list() const {
	return m_emitters;
}

void AbstractEmitterManager::attach(IEmitterNotify *notify) {
	m_notifiers.push_back(notify);

	for (const auto &emitter : m_emitters)
		notify->attached(emitter);
}

void AbstractEmitterManager::detach(IEmitterNotify *notify) {
	m_notifiers.remove_if([notify](const auto &match) -> bool {
		return match == notify;
	});

	for (const auto &emitter : m_emitters)
		notify->detached(emitter);
}
