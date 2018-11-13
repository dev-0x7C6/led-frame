#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

namespace Emitter {
namespace Concrete {

class OffEmitter final : public Abstract::AbstractEmitter {
public:
	explicit OffEmitter(ci32 id);

	virtual EmitterType type() const override;
};
} // namespace Concrete
} // namespace Emitter
