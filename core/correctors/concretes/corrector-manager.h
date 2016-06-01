#pragma once

#include <core/correctors/abstracts/abstract-corrector-manager.h>

namespace Corrector {
namespace Concrete {
namespace Manager {

class CorrectorManager final : public ::Corrector::Abstract::AbstractCorrectorManager {
public:
	explicit CorrectorManager() = default;
	virtual ~CorrectorManager() = default;
};
}
}
}
