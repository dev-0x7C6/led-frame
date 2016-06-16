#include <core/correctors/abstracts/abstract-corrector-manager.h>
#include <core/correctors/interfaces/icorrector-notify.h>

#include <algorithm>

using namespace Corrector::Abstract;
using namespace Corrector::Interface;

void AbstractCorrectorManager::attach(ICorrectorNotify *notify) {
	m_notifiers.push_back(notify);
}

void AbstractCorrectorManager::detach(ICorrectorNotify *notify) {
	m_notifiers.remove(notify);
}

void AbstractCorrectorManager::attach(const std::shared_ptr<ICorrector> &corrector) {
	auto interface = corrector.get();
	m_correctors.emplace_back(corrector);
	std::sort(m_correctors.begin(), m_correctors.end(), [](const auto &a, auto &b) {
		return (a->priority() > b->priority());
	});

	QObject::connect(interface, &INotificationCallback::notify, this, &INotificationCallback::notify);

	for (const auto &notify : m_notifiers)
		notify->attached(interface);
}

void AbstractCorrectorManager::detach(const std::shared_ptr<ICorrector> &corrector) {
	auto interface = corrector.get();

	for (const auto &notify : m_notifiers)
		notify->detached(interface);

	std::remove_if(m_correctors.begin(), m_correctors.end(), [&corrector](const auto &value) { return value == corrector; });
}

const std::vector<std::shared_ptr<ICorrector>> &AbstractCorrectorManager::correctorList() const {
	return m_correctors;
}

uint32_t AbstractCorrectorManager::execute(uint32_t color) {
	for (const auto &corrector : m_correctors)
		if (corrector->enabled())
			color = corrector->correct(color);

	return color;
}

void AbstractCorrectorManager::push() {
	for (const auto &corrector : m_correctors)
		if (corrector->enabled())
			corrector->push();
}

void AbstractCorrectorManager::pop() {
	for (const auto &corrector : m_correctors)
		if (corrector->enabled())
			corrector->pop();
}
