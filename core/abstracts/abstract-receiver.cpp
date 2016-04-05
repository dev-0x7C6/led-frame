#include <core/abstracts/abstract-receiver.h>
#include <core/interfaces/iemitter.h>

using namespace Abstract;

AbstractReceiver::AbstractReceiver() {
	m_data.fill(0);
}

void AbstractReceiver::connectEmitter(const std::shared_ptr<Interface::IEmitter> &emitter) {
	m_emitter = emitter;
}

bool AbstractReceiver::isEmitterConnected() {
	return (m_emitter != nullptr);
}

const std::shared_ptr<Interface::IEmitter> &AbstractReceiver::connectedEmitter() const {
	return m_emitter;
}

QString AbstractReceiver::name() const {
	return m_name;
}

void AbstractReceiver::setName(const QString &name) {
	m_name = name;
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

