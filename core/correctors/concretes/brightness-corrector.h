#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public ICorrector {
public:
	explicit BrightnessCorrector(i32 id, int owner);

	CorrectorType type() const noexcept final;
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
