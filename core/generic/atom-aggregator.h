#pragma once

#include <core/generic/irepresentable.h>
#include <core/generic/inotification.h>

#include <mutex>
#include <list>

class AtomAggregator {
public:
	void attach(INotification *notificator) noexcept;
	void detach(INotification *notificator) noexcept;

	void attach(const std::shared_ptr<IRepresentable> &object) noexcept;
	void detach(const std::shared_ptr<IRepresentable> &object) noexcept;
	void detach(i32 id) noexcept;

	void enumerate(const std::function<void(const std::shared_ptr<IRepresentable> &)> &callback) const noexcept;

	auto find(Category category, int id) noexcept -> std::shared_ptr<IRepresentable>;
	auto find_corrector(const int id) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Corrector, id); }
	auto find_emitter(const int id) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Emitter, id); }
	auto find_receiver(const int id) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Receiver, id); }

private:
	void notifyAll(NotifyAction, const std::shared_ptr<IRepresentable> &);

private:
	std::list<std::shared_ptr<IRepresentable>> m_objects;
	std::list<INotification *> m_notifications;
	mutable std::recursive_mutex m_mutex;
};
