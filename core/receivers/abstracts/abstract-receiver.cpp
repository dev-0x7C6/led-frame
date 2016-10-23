#include <core/receivers/abstracts/abstract-receiver.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/correctors/concretes/corrector-manager.h>

#include <memory>

using namespace Receiver::Abstract;
using namespace Corrector::Concrete::Manager;

AbstractReceiver::AbstractReceiver()
		: m_correctorManager(std::make_unique<CorrectorManager>()) {
	m_data.fill(0);
}

AbstractReceiver::~AbstractReceiver() {
	disconnectEmitter();
}

QJsonObject AbstractReceiver::parameters() const {
	return {
		{"receiver_name", name()},
		{"receiver_id", id()},
		{"receiver_emitter_connected", isEmitterConnected() ? m_emitter->id() : -1}};
}

void AbstractReceiver::disconnectEmitter() {
	if (m_emitter) m_emitter->disconnect();
}

void AbstractReceiver::connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	if (m_emitter == emitter) return;
	disconnectEmitter();

	m_emitter = emitter;
	emit notify();

	if (m_emitter)
		m_emitter->connect();
}

bool AbstractReceiver::isEmitterConnected() const {
	return (m_emitter != nullptr);
}

const std::shared_ptr<Emitter::Interface::IEmitter> &AbstractReceiver::connectedEmitter() const {
	return m_emitter;
}

QString AbstractReceiver::name() const {
	return m_name;
}

void AbstractReceiver::setName(const QString &name) {
	m_name = name;
	//TODO: We should notify when name is changed
}

Corrector::Concrete::Manager::CorrectorManager *AbstractReceiver::correctorManager() {
	return m_correctorManager.get();
}

Container::ColorScanlineContainer AbstractReceiver::scanline() { return m_emitter->data(); }

QString AbstractReceiver::emitterName() const {
	return m_emitter->name();
}
