#pragma once

#include <core/interfaces/icorrector.h>

namespace Factory {

	class CorrectorFactory final {
	public:
		explicit CorrectorFactory() = delete;
		virtual ~CorrectorFactory() = delete;

		static std::shared_ptr<Interface::ICorrector> create(const Enum::CorrectorType &type);
	};

}
