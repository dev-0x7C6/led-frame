#include <core/abstracts/abstract-receiver.h>
#include <core/interfaces/iemitter.h>

using namespace Abstract;

void AbstractReceiver::connectEmitter(const std::shared_ptr<Interface::IEmitter> &emitter) {
	m_emitter = emitter;
}

bool AbstractReceiver::isEmitterConnected() {
	return (m_emitter != nullptr);
}

Container::ColorScanlineContainer &AbstractReceiver::data() {
	if (m_emitter == nullptr)
		return m_data;

	m_data = m_emitter->data();
	return m_data;
}

