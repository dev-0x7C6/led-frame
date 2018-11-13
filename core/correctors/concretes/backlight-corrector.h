#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BacklightCorrector final : public ICorrector {
public:
	explicit BacklightCorrector(ci32 id, int owner);

	CorrectorType type() const noexcept final { return CorrectorType::Backlight; }
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
