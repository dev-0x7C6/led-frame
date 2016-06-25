#include <core/emitters/abstracts/abstract-emitter-manager.h>
#include <core/emitters/interfaces/iemitter-notify.h>

#include <QObject>

using namespace Emitter::Abstract;
using namespace Emitter::Interface;

void AbstractEmitterManager::attach(std::shared_ptr<IEmitter> &emitter) {
	auto interface = emitter.get();
	m_emitters.push_back(emitter);

	connect(interface, &INotificationCallback::notify, [this, &emitter]() {
		for (const auto &notify : m_notifiers)
			notify->modified(emitter);

		emit notify();
	});

	for (const auto &notify : m_notifiers)
		notify->attached(emitter);
}

void AbstractEmitterManager::detach(std::shared_ptr<IEmitter> &emitter) {
	for (const auto &notify : m_notifiers)
		notify->detached(emitter);

	m_emitters.remove_if([&emitter](const auto &match) -> bool {
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
