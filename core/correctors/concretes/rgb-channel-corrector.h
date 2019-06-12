#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class RedChannelCorrector final : public ICorrector {
public:
	explicit RedChannelCorrector(int owner);

	CorrectorType type() const noexcept final { return CorrectorType::RedChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};

class GreenChannelCorrector final : public ICorrector {
public:
	explicit GreenChannelCorrector(int owner);

	CorrectorType type() const noexcept final { return CorrectorType::GreenChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};

class BlueChannelCorrector final : public ICorrector {
public:
	explicit BlueChannelCorrector(int owner);

	CorrectorType type() const noexcept final { return CorrectorType::BlueChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
