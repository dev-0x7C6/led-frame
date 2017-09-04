#pragma once

#include <core/interfaces/iemitter.h>

namespace Emitter {
namespace Factory {

class EmitterFactory final {
public:
	explicit EmitterFactory() = delete;
	virtual ~EmitterFactory() = delete;

	static std::shared_ptr<Emitter::Interface::IEmitter> create(const Enum::EmitterType &type);
	static std::shared_ptr<Emitter::Interface::IEmitter> create(const Enum::EmitterType &type, const QString &name);
};
}
}
