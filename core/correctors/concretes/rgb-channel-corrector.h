#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class RedChannelCorrector final : public Interface::ICorrector {
public:
	explicit RedChannelCorrector(ci32 id, int owner);
	virtual ~RedChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};

class GreenChannelCorrector final : public Interface::ICorrector {
public:
	explicit GreenChannelCorrector(ci32 id, int owner);
	virtual ~GreenChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};

class BlueChannelCorrector final : public Interface::ICorrector {
public:
	explicit BlueChannelCorrector(ci32 id, int owner);
	virtual ~BlueChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};

class BacklightCorrector final : public Interface::ICorrector {
public:
	explicit BacklightCorrector(ci32 id, int owner);
	virtual ~BacklightCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
