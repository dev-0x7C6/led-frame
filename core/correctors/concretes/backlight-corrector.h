#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BacklightCorrector final : public Interface::ICorrector {
public:
	explicit BacklightCorrector(ci32 id, int owner);
	virtual ~BacklightCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
