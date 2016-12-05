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
	m_correctors.emplace_back(corrector);
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
	auto interface = corrector.get();

	for (const auto &notify : m_notifiers)
		notify->detached(interface);

	std::remove_if(m_correctors.begin(), m_correctors.end(), [&corrector](const auto &value) { return value == corrector; });
}

std::shared_ptr<ICorrector> AbstractCorrectorManager::find(const int id) const {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector->id() == id)
			return corrector;

	return nullptr;
}

void AbstractCorrectorManager::enumerate(std::function<void(const std::shared_ptr<ICorrector> &)> callback) const {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		callback(corrector);
}

uint32_t AbstractCorrectorManager::execute(color value) {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector->isEnabled())
			value = corrector->correct(value);

	return value;
}

void AbstractCorrectorManager::push() {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector->isEnabled())
			corrector->push();
}

void AbstractCorrectorManager::pop() {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &corrector : m_correctors)
		if (corrector->isEnabled())
			corrector->pop();
}
