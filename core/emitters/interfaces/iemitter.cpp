#include "iemitter.h"

#include <iostream>

using namespace Emitter::Interface;

IEmitter::IEmitter(ci32 id)
		: INotify(id) {}

IEmitter::~IEmitter() = default;

void IEmitter::commit(const Container::Scanline &scanline) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	m_data = scanline;
	m_firstFrameReady = true;
}

auto IEmitter::data() const noexcept -> Container::Scanline {
	std::lock_guard<std::mutex> _(m_mutex);
	return m_data;
}

auto IEmitter::acquire() noexcept -> std::unique_ptr<Functional::RaiiReferenceCounter> {
	return std::make_unique<Functional::RaiiReferenceCounter>(m_counter);
}

auto IEmitter::usages() const noexcept -> int { return m_counter; }

void IEmitter::interpret(any data) noexcept {
	std::cout << "default implementation: " << data.type().name() << std::endl;
}
bool IEmitter::isFirstFrameReady() const noexcept { return m_firstFrameReady; }
