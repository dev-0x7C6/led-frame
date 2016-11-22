#pragma once

#include <core/correctors/interfaces/icorrector.h>

#include <functional>
#include <list>
#include <memory>
#include <vector>

namespace Corrector {
namespace Interface {

class ICorrectorNotify;

class ICorrectorManager : public ::Interface::INotificationCallback {
public:
	explicit ICorrectorManager() = default;
	~ICorrectorManager() override = default;

	virtual void attach(ICorrectorNotify *notify) = 0;
	virtual void detach(ICorrectorNotify *notify) = 0;

	virtual void attach(const std::shared_ptr<ICorrector> &corrector) = 0;
	virtual void detach(const std::shared_ptr<ICorrector> &corrector) = 0;

	virtual std::shared_ptr<ICorrector> find(const int id) const = 0;
	virtual void enumerate(std::function<void(const std::shared_ptr<ICorrector> &)> callback) const = 0;

	virtual uint32_t execute(uint32_t color) = 0;
	virtual void push() = 0;
	virtual void pop() = 0;
};
}
}
