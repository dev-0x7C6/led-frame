#pragma once

#include <core/correctors/interfaces/icorrector-manager.h>
#include <core/correctors/interfaces/icorrector.h>

#include <list>
#include <memory>
#include <vector>

namespace Corrector {
namespace Abstract {

class AbstractCorrectorManager : public Corrector::Interface::ICorrectorManager {
public:
	explicit AbstractCorrectorManager() = default;
	virtual ~AbstractCorrectorManager() = default;

	virtual void attach(Corrector::Interface::ICorrectorNotify *notify) override;
	virtual void detach(Corrector::Interface::ICorrectorNotify *notify) override;

	virtual void attach(const std::shared_ptr<Corrector::Interface::ICorrector> &corrector) override;
	virtual void detach(const std::shared_ptr<Corrector::Interface::ICorrector> &corrector) override;

	virtual const std::vector<std::shared_ptr<Corrector::Interface::ICorrector>> &correctorList() const override;

	virtual uint32_t execute(uint32_t color) override;
	virtual void push() override;
	virtual void pop() override;

private:
	std::vector<std::shared_ptr<Corrector::Interface::ICorrector>> m_correctors;
	std::list<Corrector::Interface::ICorrectorNotify *> m_notifiers;
};
}
}
