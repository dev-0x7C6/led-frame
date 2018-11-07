#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class ColorEnhancerCorrector final : public ICorrector {
public:
	explicit ColorEnhancerCorrector(ci32 id, int owner);

	CorrectorType type() const noexcept final { return CorrectorType::ColorEnhancer; }
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
