#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

namespace Emitter {
namespace Concrete {

class OffEmitter final : public Abstract::AbstractEmitter {
public:
	explicit OffEmitter();

	EmitterType type() const final { return EmitterType::Off; }
};
} // namespace Concrete
} // namespace Emitter
