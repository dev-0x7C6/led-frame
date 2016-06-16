#pragma once

#include <core/correctors/interfaces/icorrector.h>

#include <list>
#include <memory>
#include <vector>

namespace Corrector {
namespace Interface {

class ICorrectorNotify;

class ICorrectorManager : public ::Interface::INotificationCallback {
public:
	explicit ICorrectorManager() = default;
	virtual ~ICorrectorManager() = default;

	virtual void attach(ICorrectorNotify *notify) = 0;
	virtual void detach(ICorrectorNotify *notify) = 0;

	virtual void attach(const std::shared_ptr<ICorrector> &corrector) = 0;
	virtual void detach(const std::shared_ptr<ICorrector> &corrector) = 0;

	virtual const std::vector<std::shared_ptr<ICorrector>> &correctorList() const = 0;

	virtual uint32_t execute(uint32_t color) = 0;
	virtual void push() = 0;
	virtual void pop() = 0;
};
}
}
