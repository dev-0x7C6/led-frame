#pragma once

#include <memory>

class IEmitter;
enum class EmitterType;

namespace Factory {

std::shared_ptr<IEmitter> make_emitter(EmitterType type) noexcept;
std::shared_ptr<IEmitter> make_emitter(EmitterType type, std::string &&name) noexcept;

} // namespace Factory
