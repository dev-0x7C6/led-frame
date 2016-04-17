#include <core/abstracts/abstract-emitter.h>

using namespace Abstract;

AbstractEmitter::AbstractEmitter() : m_connectionCount(0) {
	m_data.fill(0);
}

void AbstractEmitter::commit(const Container::ColorScanlineContainer &scanline) {
	std::lock_guard<std::mutex> _(m_mutex);
	m_data = scanline;
}

Container::ColorScanlineContainer AbstractEmitter::data() {
	std::lock_guard<std::mutex> _(m_mutex);
	return m_data;
}

QString AbstractEmitter::name() const {
	return m_name;
}

void AbstractEmitter::setName(const QString &name) {
	m_name = name;
}

void AbstractEmitter::connect() {
	m_connectionCount++;
	onConnect(m_connectionCount);
}

void AbstractEmitter::disconnect() {
	m_connectionCount--;
	onDisconnect(m_connectionCount);
}

uint32_t AbstractEmitter::connectionCount() {
	return m_connectionCount;
}
