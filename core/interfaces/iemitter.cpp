#include "iemitter.h"

#include <iostream>

IEmitter::~IEmitter() {
	invalidate();
}

auto IEmitter::data() const noexcept -> Container::Scanline {
	return m_scanline.get().value_or(Container::Scanline());
}

auto IEmitter::acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter> {
	return std::make_unique<Functional::RaiiReferenceCounter>(m_counter);
}

auto IEmitter::usages() const noexcept -> int { return m_counter; }

void IEmitter::interpret(std::any data) noexcept {
	std::cout << "default implementation: " << data.type().name() << std::endl;
}

bool IEmitter::isFirstFrameReady() const noexcept { return m_scanline.get().has_value(); }
