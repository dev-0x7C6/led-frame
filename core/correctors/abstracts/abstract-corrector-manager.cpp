#include <core/correctors/abstracts/abstract-corrector-manager.h>
#include <core/correctors/interfaces/icorrector-notify.h>

#include <algorithm>

using namespace Corrector::Abstract;
using namespace Corrector::Interface;

void AbstractCorrectorManager::attach(ICorrectorNotify *notify) {
	std::lock_guard<std::mutex> _(m_mutex);
	m_notifiers.push_back(notify);

	for (const auto &corrector : m_correctors)
		notify->attached(corrector.get());
}

void AbstractCorrectorManager::detach(ICorrectorNotify *notify) {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		notify->detached(corrector.get());

	m_notifiers.remove(notify);
}

void AbstractCorrectorManager::attach(const std::shared_ptr<ICorrector> &corrector) {
	std::lock_guard<std::mutex> _(m_mutex);
	auto interface = corrector.get();
	m_correctors.push_back(corrector);

	std::sort(m_correctors.begin(), m_correctors.end(), [](const auto &a, const auto &b) {
		return (a->priority() > b->priority());
	});

	connect(interface, &INotify::notify, [this, interface]() {
		for (const auto &notify : m_notifiers)
			notify->modified(interface);

		emit notify();
	});

	for (const auto &notify : m_notifiers)
		notify->attached(interface);
}

void AbstractCorrectorManager::detach(const std::shared_ptr<ICorrector> &corrector) {
	std::lock_guard<std::mutex> _(m_mutex);
	auto keep_for_peaceful_decay = corrector;

	m_correctors.erase(
		std::remove_if(m_correctors.begin(), m_correctors.end(),
			[&corrector](const auto &value) { return value == corrector; }),
		m_correctors.end());

	for (const auto &notify : m_notifiers)
		notify->detached(keep_for_peaceful_decay.get());
}

std::shared_ptr<ICorrector> AbstractCorrectorManager::find(const int id) const {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector != nullptr && corrector->id() == id)
			return corrector;

	return nullptr;
}

void AbstractCorrectorManager::enumerate(std::function<void(const std::shared_ptr<ICorrector> &)> callback) const {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		callback(corrector);
}

void AbstractCorrectorManager::execute(Container::Scanline &scanline) {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector != nullptr && corrector->isEnabled())
			corrector->correct(scanline);
}
