#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Factory {

class CorrectorFactory final {
public:
	explicit CorrectorFactory() = delete;
	virtual ~CorrectorFactory() = delete;

	static std::shared_ptr<Corrector::Interface::ICorrector> create(const Enum::CorrectorType &type, int owner);
};
}
}
