#include "atom-aggregator.h"

#include <algorithm>

void AtomAggregator::notifyAll(NotifyAction action, const std::shared_ptr<IRepresentable> &object) {
	std::lock_guard _(m_mutex);
	for (auto &&notification : m_notifications)
		notification->action(action, object);
}

void AtomAggregator::attach(INotification *notificator) noexcept {
	std::lock_guard _(m_mutex);
	m_notifications.emplace_back(notificator);

	for (const auto &object : m_objects)
		notificator->action(NotifyAction::Attached, object);
}

void AtomAggregator::detach(INotification *notificator) noexcept {
	std::lock_guard _(m_mutex);
	m_notifications.remove_if([notificator](auto &&element) { return element == notificator; });
}

void AtomAggregator::attach(const std::shared_ptr<IRepresentable> &object) noexcept {
	std::lock_guard _(m_mutex);
	m_objects.emplace_back(object);
	notifyAll(NotifyAction::Attached, object);

	object->attach([this, weak = std::weak_ptr(object)]() {
		std::lock_guard _(m_mutex);
		auto shared = weak.lock();

		if (shared)
			notifyAll(NotifyAction::Modified, shared);
	});
}

void AtomAggregator::detach(const std::shared_ptr<IRepresentable> &object) noexcept {
	std::lock_guard _(m_mutex);
	notifyAll(NotifyAction::Detached, object);

	m_objects.remove_if([ptr = object.get()](auto &&element) {
		return element.get() == ptr;
	});
}

void AtomAggregator::detach(i32 id) noexcept {
	std::lock_guard _(m_mutex);
	m_objects.remove_if([id](auto &&element) {
		return element->id() == id;
	});
}

void AtomAggregator::enumerate(const std::function<void(const std::shared_ptr<IRepresentable> &)> &callback) const noexcept {
	std::lock_guard _(m_mutex);
	for (const auto &object : m_objects)
		callback(object);
}

auto AtomAggregator::find(const Category category, const int id) noexcept -> std::shared_ptr<IRepresentable> {
	std::lock_guard _(m_mutex);
	for (const auto &object : m_objects)
		if (id == object->id() && category == object->category())
			return object;

	return nullptr;
}
