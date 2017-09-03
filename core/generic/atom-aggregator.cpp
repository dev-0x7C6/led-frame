#include "atom-aggregator.h"

#include <algorithm>

void AtomAggregator::attach(INotification *notificator) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	m_notifications.emplace_back(notificator);

	for (const auto &object : m_objects)
		notificator->action(NotifyAction::Attached, object);
}

void AtomAggregator::detach(INotification *notificator) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	m_notifications.erase(std::remove_if(m_notifications.begin(),
							  m_notifications.end(),
							  [notificator](const auto &value) { return value == notificator; }),
		m_notifications.end());
}

void AtomAggregator::attach(const std::shared_ptr<IAtom> &object) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	m_objects.emplace_back(object);

	for (auto notification : m_notifications)
		notification->action(NotifyAction::Attached, object);

	object->attach([ this, object = object ]() {
		std::lock_guard<std::mutex> locker(m_mutex);
		for (auto notification : m_notifications)
			notification->action(NotifyAction::Modified, object);
	});
}

void AtomAggregator::detach(const std::shared_ptr<IAtom> &object) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);

	for (auto notification : m_notifications)
		notification->action(NotifyAction::Detached, object);

	m_objects.erase(std::remove_if(m_objects.begin(),
						m_objects.end(),
						[ptr = object.get()](const auto &value) { return ptr == value.get(); }),
		m_objects.end());
}

void AtomAggregator::enumerate(std::function<void(const std::shared_ptr<IAtom> &)> callback) const noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &object : m_objects)
		callback(object);
}

auto AtomAggregator::find(const Category category, const int id) noexcept -> std::shared_ptr<IAtom> {
	std::lock_guard<std::mutex> _(m_mutex);
	for (const auto &object : m_objects)
		if (id == object->id() && category == object->category())
			return object;

	return nullptr;
}
