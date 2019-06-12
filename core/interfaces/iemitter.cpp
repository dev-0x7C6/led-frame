#include "iemitter.h"

#include <iostream>

void IEmitter::commit(const Container::Scanline &scanline) noexcept {
	m_scanline.set(scanline);
	m_firstFrameReady = 1;
}

void IEmitter::commit(Container::Scanline &&scanline) noexcept {
	m_scanline.set(scanline);
	m_firstFrameReady = 1;
}

auto IEmitter::data() const noexcept -> Container::Scanline {
	return m_scanline.get();
}

auto IEmitter::acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter> {
	return std::make_unique<Functional::RaiiReferenceCounter>(m_counter);
}

auto IEmitter::usages() const noexcept -> int { return m_counter; }

void IEmitter::interpret(std::any data) noexcept {
	std::cout << "default implementation: " << data.type().name() << std::endl;
}
bool IEmitter::isFirstFrameReady() const noexcept { return m_firstFrameReady == 1; }
