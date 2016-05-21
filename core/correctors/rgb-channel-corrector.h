#pragma once

#include <core/abstracts/abstract-corrector.h>

namespace Corrector {

class RGBChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit RGBChannelCorrector();
	virtual ~RGBChannelCorrector() = default;

	virtual Enum::CorrectorType type() override;
	virtual uint32_t correct(const uint32_t &color) override;

	double redFactor() const;
	double greenFactor() const;
	double blueFactor() const;

	void setRedFactor(const float &factor);
	void setGreenFactor(const float &factor);
	void setBlueFactor(const float &factor);

private:
	std::atomic<float> m_rfactor;
	std::atomic<float> m_gfactor;
	std::atomic<float> m_bfactor;
};
}
