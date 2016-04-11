#pragma once

#include <core/interfaces/icolor-corrector.h>

namespace Factory {

	class CorrectorFactory final {
	public:
		explicit CorrectorFactory() = delete;
		virtual ~CorrectorFactory() = delete;

		static std::shared_ptr<Interface::IColorCorrector> create(const Enum::CorrectorType &type);
	};

}
