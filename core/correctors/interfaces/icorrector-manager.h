#pragma once

#include <core/correctors/interfaces/icorrector.h>

#include <list>
#include <memory>

namespace Corrector {
namespace Interface {

class ICorrectorNotify;

class ICorrectorManager {
public:
	explicit ICorrectorManager() = default;
	virtual ~ICorrectorManager() = default;

	virtual void attach(ICorrectorNotify *notify) = 0;
	virtual void detach(ICorrectorNotify *notify) = 0;

	virtual void attach(const std::shared_ptr<ICorrector> &corrector) = 0;
	virtual void detach(const std::shared_ptr<ICorrector> &corrector) = 0;

	virtual const std::list<std::shared_ptr<ICorrector>> &correctorList() const = 0;

protected:
	virtual uint32_t execute(const uint32_t &color) = 0;
};
}
}
