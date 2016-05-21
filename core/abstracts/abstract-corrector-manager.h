#pragma once

#include <core/interfaces/icorrector-manager.h>
#include <core/interfaces/icorrector.h>

#include <list>
#include <memory>
#include <vector>

namespace Abstract {

class AbstractCorrectorManager : public Interface::ICorrectorManager {
public:
	explicit AbstractCorrectorManager() = default;
	virtual ~AbstractCorrectorManager() = default;

	virtual void attach(Interface::ICorrectorNotify *notify) override;
	virtual void detach(Interface::ICorrectorNotify *notify) override;

	virtual void attach(const std::shared_ptr<Interface::ICorrector> &corrector) override;
	virtual void detach(const std::shared_ptr<Interface::ICorrector> &corrector) override;

	virtual const std::list<std::shared_ptr<Interface::ICorrector>> &correctorList() const override;

protected:
	virtual uint32_t execute(const uint32_t &color) override;

private:
	std::list<std::shared_ptr<Interface::ICorrector>> m_correctors;
	std::list<Interface::ICorrectorNotify *> m_notifiers;
};
}
