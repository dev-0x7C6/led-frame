#include <core/abstracts/abstract-emitter.h>

using namespace Abstract;

AbstractEmitter::AbstractEmitter() {
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
