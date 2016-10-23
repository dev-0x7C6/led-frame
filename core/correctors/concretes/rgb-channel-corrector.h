#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class RGBChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit RGBChannelCorrector(const int owner);
	virtual ~RGBChannelCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(uint32_t color) override;

	virtual double minimumFactor() const override;
	virtual double maximumFactor() const override;

	double redFactor() const;
	double greenFactor() const;
	double blueFactor() const;

	void setRedFactor(double factor);
	void setGreenFactor(double factor);
	void setBlueFactor(double factor);

private:
	std::atomic<double> m_rfactor;
	std::atomic<double> m_gfactor;
	std::atomic<double> m_bfactor;
};
}
}
