#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class GammaCorrector final : public ICorrector {
public:
	explicit GammaCorrector(int owner);

	CorrectorType type() const noexcept final { return CorrectorType::Gamma; }
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
