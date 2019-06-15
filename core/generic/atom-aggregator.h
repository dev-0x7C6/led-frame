#pragma once

#include <core/generic/irepresentable.h>
#include <core/generic/inotification.h>

#include <mutex>
#include <list>

struct corrector_type {};
struct emitter_type {};
struct receiver_type {};

class AtomAggregator {
public:
	void attach(INotification *notificator) noexcept;
	void detach(INotification *notificator) noexcept;

	void attach(const std::shared_ptr<IRepresentable> &object) noexcept;
	void detach(const std::shared_ptr<IRepresentable> &object) noexcept;
	void detach(i32 id) noexcept;

	void enumerate(const std::function<void(const std::shared_ptr<IRepresentable> &)> &callback) const noexcept;

	auto find(Category category, int id) noexcept -> std::shared_ptr<IRepresentable>;
	auto find(const int id, corrector_type) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Corrector, id); }
	auto find(const int id, emitter_type) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Emitter, id); }
	auto find(const int id, receiver_type) noexcept -> std::shared_ptr<IRepresentable> { return find(Category::Receiver, id); }

private:
	void notifyAll(NotifyAction, const std::shared_ptr<IRepresentable> &);

private:
	std::list<std::shared_ptr<IRepresentable>> m_objects;
	std::list<INotification *> m_notifications;
	mutable std::recursive_mutex m_mutex;
};
