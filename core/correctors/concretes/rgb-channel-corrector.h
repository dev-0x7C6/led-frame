#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

constexpr auto RGB_CORRECTION_MIN_VALUE = 0.0;
constexpr auto RGB_CORRECTION_MAX_VALUE = 1.0;

class RedChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit RedChannelCorrector(int id, int owner);
	virtual ~RedChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(color value) override;

	virtual double minimumFactor() const override { return RGB_CORRECTION_MIN_VALUE; }
	virtual double maximumFactor() const override { return RGB_CORRECTION_MAX_VALUE; }
};

class GreenChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit GreenChannelCorrector(int id, int owner);
	virtual ~GreenChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(color value) override;

	virtual double minimumFactor() const override { return RGB_CORRECTION_MIN_VALUE; }
	virtual double maximumFactor() const override { return RGB_CORRECTION_MAX_VALUE; }
};

class BlueChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit BlueChannelCorrector(int id, int owner);
	virtual ~BlueChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(color value) override;

	virtual double minimumFactor() const override { return RGB_CORRECTION_MIN_VALUE; }
	virtual double maximumFactor() const override { return RGB_CORRECTION_MAX_VALUE; }
};
}
}
