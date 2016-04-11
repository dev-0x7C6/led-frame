#pragma once

#include <core/abstracts/abstract-corrector.h>

namespace Corrector {

	class RGBChannelCorrector final : public Abstract::AbstractCorrector {
	public:
		explicit RGBChannelCorrector() = default;
		virtual ~RGBChannelCorrector() = default;

		virtual Enum::CorrectorType type() override;
		virtual uint32_t correct(const uint32_t &color) override;

		double redFactor() const;
		double greenFactor() const;
		double blueFactor() const;

		void setRedFactor(const double &factor);
		void setGreenFactor(const double &factor);
		void setBlueFactor(const double &factor);

	private:
		std::atomic<double> m_rfactor;
		std::atomic<double> m_gfactor;
		std::atomic<double> m_bfactor;
	};

}
