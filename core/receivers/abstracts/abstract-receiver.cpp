#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/interfaces/iemitter.h>

#include <memory>
#include <atomic>

//TODO: RPI have older stdlib and does not support atomic_load and atomic_exchange for shared_ptr

using namespace Receiver::Abstract;

AbstractReceiver::AbstractReceiver(const i32 id)
		: IReceiver(id) {
	m_data.fill(0);
}

AbstractReceiver::~AbstractReceiver() = default;

void AbstractReceiver::connectEmitter(const std::shared_ptr<IEmitter> &emitter) {
	if (m_emitter == emitter) return;

#ifdef RPI
	m_emitter = emitter;
#else
	std::atomic_exchange(&m_emitter, emitter);
#endif

	notify();

	if (m_emitter)
		m_acquiredEmitter = m_emitter->acquire(); //TODO: acquaire or emitter use_count() ?
}

bool AbstractReceiver::isEmitterConnected() const {
#ifdef RPI
	return m_emitter.get() != nullptr;
#else
	return std::atomic_load(&m_emitter) != nullptr;
#endif
}

std::shared_ptr<IEmitter> AbstractReceiver::connectedEmitter() const {
#ifdef RPI
	return m_emitter;
#else
	return std::atomic_load(&m_emitter);
#endif
}

int AbstractReceiver::connectedEmitterId() const {
	if (m_emitter)
		return m_emitter->id();

	return -1;
}

auto AbstractReceiver::name() const noexcept -> std::string {
	return m_name;
}

void AbstractReceiver::setName(const std::string &name) {
	m_name = name;
	//TODO: We should notify when name is changed
}

auto AbstractReceiver::correctors() noexcept -> AtomAggregator & {
	return m_correctors;
}

std::string AbstractReceiver::emitterName() const {
	return m_emitter->name();
}
