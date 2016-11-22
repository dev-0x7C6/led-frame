#pragma once

#include <core/correctors/interfaces/icorrector-manager.h>
#include <core/correctors/interfaces/icorrector.h>

#include <list>
#include <memory>
#include <mutex>
#include <vector>

namespace Corrector {
namespace Abstract {

class AbstractCorrectorManager : public Corrector::Interface::ICorrectorManager {
public:
	explicit AbstractCorrectorManager() = default;
	~AbstractCorrectorManager() override = default;

	void attach(Corrector::Interface::ICorrectorNotify *notify) override;
	void detach(Corrector::Interface::ICorrectorNotify *notify) override;

	void attach(const std::shared_ptr<Corrector::Interface::ICorrector> &corrector) override;
	void detach(const std::shared_ptr<Corrector::Interface::ICorrector> &corrector) override;

	std::shared_ptr<Interface::ICorrector> find(const int id) const override;
	void enumerate(std::function<void(const std::shared_ptr<Interface::ICorrector> &)> callback) const override;

	uint32_t execute(uint32_t color) override;
	void push() override;
	void pop() override;

private:
	std::vector<std::shared_ptr<Corrector::Interface::ICorrector>> m_correctors;
	std::list<Corrector::Interface::ICorrectorNotify *> m_notifiers;
	mutable std::mutex m_mutex;
};
}
}
