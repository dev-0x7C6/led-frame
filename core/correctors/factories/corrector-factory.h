#pragma once

#include <core/interfaces/icorrector.h>
#include <memory>

namespace Corrector {
namespace Factory {

class CorrectorFactory final {
public:
	explicit CorrectorFactory() = delete;
	virtual ~CorrectorFactory() = delete;

	static std::shared_ptr<ICorrector> create(const Enum::CorrectorType &type, int owner);
};
}
}
