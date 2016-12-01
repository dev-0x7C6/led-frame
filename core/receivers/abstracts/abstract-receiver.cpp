#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/correctors/concretes/corrector-manager.h>

#include <memory>
#include <atomic>

using namespace Receiver::Abstract;
using namespace Corrector::Concrete;

AbstractReceiver::AbstractReceiver(int id)
		: IReceiver(id) {
	m_data.fill(0);
}

AbstractReceiver::~AbstractReceiver() {
	disconnectEmitter();
}

void AbstractReceiver::disconnectEmitter() {
	m_acquiredEmitter = nullptr;
}

void AbstractReceiver::connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	if (m_emitter == emitter) return;
	disconnectEmitter();

	std::atomic_exchange(&m_emitter, emitter);
	emit notify();

	if (m_emitter)
		m_acquiredEmitter = m_emitter->acquire();
}

bool AbstractReceiver::isEmitterConnected() const {
	return std::atomic_load(&m_emitter) != nullptr;
}

std::shared_ptr<Emitter::Interface::IEmitter> AbstractReceiver::connectedEmitter() const {
	return std::atomic_load(&m_emitter);
}

int AbstractReceiver::connectedEmitterId() const {
	if (m_emitter)
		return m_emitter->id();

	return -1;
}

QString AbstractReceiver::name() const {
	return m_name;
}

void AbstractReceiver::setName(const QString &name) {
	m_name = name;
	//TODO: We should notify when name is changed
}

Corrector::Concrete::CorrectorManager &AbstractReceiver::correctorManager() {
	return m_correctorManager;
}

QString AbstractReceiver::emitterName() const {
	return m_emitter->name();
}
