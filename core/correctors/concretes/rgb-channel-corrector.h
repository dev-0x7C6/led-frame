#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class RedChannelCorrector final : public ICorrector {
public:
	explicit RedChannelCorrector(i32 id, int owner);

	CorrectorType type() const noexcept final { return CorrectorType::RedChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};

class GreenChannelCorrector final : public ICorrector {
public:
	explicit GreenChannelCorrector(i32 id, int owner);

	CorrectorType type() const noexcept final { return CorrectorType::GreenChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};

class BlueChannelCorrector final : public ICorrector {
public:
	explicit BlueChannelCorrector(i32 id, int owner);

	CorrectorType type() const noexcept final { return CorrectorType::BlueChannel; }
	void correct(Container::Scanline &scanline) const noexcept final;
};
} // namespace Concrete
} // namespace Corrector
