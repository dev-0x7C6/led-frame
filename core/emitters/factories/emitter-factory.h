#pragma once

#include <core/emitters/interfaces/iemitter.h>

namespace Emitter {
namespace Factory {

class EmitterFactory final {
public:
	explicit EmitterFactory() = default;
	virtual ~EmitterFactory() = default;

	static std::shared_ptr<Emitter::Interface::IEmitter> create(const Enum::EmitterType &type);
	static std::shared_ptr<Emitter::Interface::IEmitter> create(const Enum::EmitterType &type, const QString &name);
};
}
}
