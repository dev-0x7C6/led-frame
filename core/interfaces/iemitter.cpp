#include "iemitter.h"

#include <iostream>

IEmitter::IEmitter(const i32 id)
		: IAtom(id) {}

void IEmitter::commit(const Container::Scanline &scanline) noexcept {
	std::lock_guard _(m_mutex);
	m_data = scanline;
	m_firstFrameReady = 1;
}

auto IEmitter::data() const noexcept -> Container::Scanline {
	std::lock_guard _(m_mutex);
	return m_data;
}

auto IEmitter::acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter> {
	return std::make_unique<Functional::RaiiReferenceCounter>(m_counter);
}

auto IEmitter::usages() const noexcept -> int { return m_counter; }

void IEmitter::interpret(std::any data) noexcept {
	std::cout << "default implementation: " << data.type().name() << std::endl;
}
bool IEmitter::isFirstFrameReady() const noexcept { return m_firstFrameReady == 1; }
