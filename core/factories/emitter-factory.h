#pragma once

#include <core/interfaces/iemitter.h>

namespace Factory {

class EmitterFactory final {
public:
	explicit EmitterFactory() = default;
	virtual ~EmitterFactory() = default;

	static std::shared_ptr<Interface::IEmitter> create(const Enum::EmitterType &type);
};
}
