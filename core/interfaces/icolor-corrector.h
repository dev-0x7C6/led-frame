#pragma once

#include <core/containers/color-correction-container.h>
#include <atomic>

namespace Interface {

	class IColorCorrector {
	public:
		explicit IColorCorrector();
		virtual ~IColorCorrector() = default;

		Container::ColorCorrectionContainer &correction();

	private:
		Container::ColorCorrectionContainer m_correction;
	};

}
