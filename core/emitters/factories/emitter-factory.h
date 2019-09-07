#pragma once

#include <memory>
#include <any>

class IEmitter;
enum class EmitterType;

namespace Factory {

std::unique_ptr<IEmitter> make_emitter(EmitterType type, std::any &&argument = {}) noexcept;
std::unique_ptr<IEmitter> make_emitter(EmitterType type, std::string &&name, std::any &&argument = {}) noexcept;

} // namespace Factory
