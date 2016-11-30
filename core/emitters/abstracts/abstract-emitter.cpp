#include <core/emitters/abstracts/abstract-emitter.h>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Emitter::Abstract;

AbstractEmitter::AbstractEmitter(int id)
		: IEmitter(id)
		, m_connectionCount(0)

{
	m_data.fill(0);
}

void AbstractEmitter::commit(const Container::ColorScanlineContainer &scanline) {
	std::lock_guard<std::mutex> _(m_mutex);
	m_data = scanline;
	m_firstFrameReady = true;
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
#ifdef QT_DEBUG
	std::cout << "[" << m_name.toStdString() << "] emitter: "
			  << "connection ref. " << m_connectionCount << std::endl;
#endif
}

void AbstractEmitter::disconnect() {
	m_connectionCount--;
	onDisconnect(m_connectionCount);
#ifdef QT_DEBUG
	std::cout << "[" << m_name.toStdString() << "] emitter: "
			  << "connection ref. " << m_connectionCount << std::endl;
#endif
}

uint32_t AbstractEmitter::connectionCount() {
	return m_connectionCount;
}
