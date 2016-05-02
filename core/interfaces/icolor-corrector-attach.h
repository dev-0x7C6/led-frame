#pragma once

#include <core/interfaces/icolor-corrector.h>

#include <vector>
#include <memory>

namespace Interface {

	class IColorCorrectorAttach {
	public:
		explicit IColorCorrectorAttach() = default;
		virtual ~IColorCorrectorAttach() = default;

		virtual void attach(const std::shared_ptr<IColorCorrector> &pair) = 0;
		virtual const std::vector<std::shared_ptr<IColorCorrector>> &list() const = 0;

	protected:
		virtual uint32_t execute(const uint32_t &color) = 0;

	};

}
