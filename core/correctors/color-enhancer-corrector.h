#pragma once

#include <core/abstracts/abstract-corrector.h>

namespace Corrector {

	class ColorEnhancerCorrector final : public Abstract::AbstractCorrector {
	public:
		explicit ColorEnhancerCorrector();
		virtual ~ColorEnhancerCorrector() = default;

		virtual Enum::CorrectorType type() override;
		virtual uint32_t correct(const uint32_t &color) override;

		double factor() const;
		void setFactor(const double &factor);

	private:
		std::atomic<double> m_factor;

	};

}
