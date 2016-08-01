#include <core/correctors/abstracts/abstract-corrector-manager.h>
#include <core/correctors/interfaces/icorrector-notify.h>

#include <algorithm>

using namespace Corrector::Abstract;
using namespace Corrector::Interface;

void AbstractCorrectorManager::attach(ICorrectorNotify *notify) {
	m_notifiers.push_back(notify);

	for (const auto &corrector : m_correctors)
		notify->attached(corrector.get());
}

void AbstractCorrectorManager::detach(ICorrectorNotify *notify) {
	for (const auto &corrector : m_correctors)
		notify->detached(corrector.get());

	m_notifiers.remove(notify);
}

void AbstractCorrectorManager::attach(const std::shared_ptr<ICorrector> &corrector) {
	auto interface = corrector.get();
	m_correctors.emplace_back(corrector);
	std::sort(m_correctors.begin(), m_correctors.end(), [](const auto &a, const auto &b) {
		return (a->priority() > b->priority());
	});

	connect(interface, &INotificationCallback::notify, [this, interface]() {
		for (const auto &notify : m_notifiers)
			notify->modified(interface);

		emit notify();
	});

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
		if (corrector->isEnabled())
			color = corrector->correct(color);

	return color;
}

void AbstractCorrectorManager::push() {
	for (const auto &corrector : m_correctors)
		if (corrector->isEnabled())
			corrector->push();
}

void AbstractCorrectorManager::pop() {
	for (const auto &corrector : m_correctors)
		if (corrector->isEnabled())
			corrector->pop();
}
