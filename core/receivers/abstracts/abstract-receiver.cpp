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
	connectEmitter(nullptr);
}

void AbstractReceiver::connectEmitter(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	if (m_emitter)
		m_emitter->disconnect();

	m_emitter = emitter;

	if (m_emitter)
		m_emitter->connect();

	if (m_callback)
		m_callback();
}

bool AbstractReceiver::isEmitterConnected() {
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

	if (m_callback)
		m_callback();
}

void AbstractReceiver::changed(const std::function<void()> &callback) {
	m_callback = callback;
}

Corrector::Concrete::Manager::CorrectorManager *AbstractReceiver::correctorManager() {
	return m_correctorManager.get();
}

Container::ColorScanlineContainer &AbstractReceiver::data() {
	if (m_emitter == nullptr)
		return m_data;

	m_data = m_emitter->data();
	return m_data;
}

const Container::ColorScanlineContainer &AbstractReceiver::constData() {
	if (m_emitter == nullptr)
		return m_data;

	m_data = m_emitter->data();
	return m_data;
}
