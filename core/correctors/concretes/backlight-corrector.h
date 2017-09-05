#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BacklightCorrector final : public ICorrector {
public:
	explicit BacklightCorrector(ci32 id, int owner);
	virtual ~BacklightCorrector() = default;

	virtual CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
