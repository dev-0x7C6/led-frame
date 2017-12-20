#pragma once

#include <core/interfaces/iemitter.h>

namespace Emitter {
namespace Factory {

class EmitterFactory final {
public:
	explicit EmitterFactory() = delete;
	virtual ~EmitterFactory() = delete;

	static std::shared_ptr<IEmitter> create(const EmitterType &type);
	static std::shared_ptr<IEmitter> create(const EmitterType &type, const std::string &name);
};
}
}
