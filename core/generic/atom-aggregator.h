#pragma once

#include <core/generic/iatom.h>
#include <core/generic/inotification.h>

#include <mutex>

struct corrector_type {};
struct emitter_type {};
struct receiver_type {};

class AtomAggregator {
public:
	void attach(INotification *notificator) noexcept;
	void detach(INotification *notificator) noexcept;

	void attach(const std::shared_ptr<IAtom> &object) noexcept;
	void detach(const std::shared_ptr<IAtom> &object) noexcept;

	void enumerate(const std::function<void(const std::shared_ptr<IAtom> &)> &callback) const noexcept;

	auto find(Category category, int id) noexcept -> std::shared_ptr<IAtom>;
	auto find(const int id, corrector_type) noexcept -> std::shared_ptr<IAtom> { return find(Category::Corrector, id); }
	auto find(const int id, emitter_type) noexcept -> std::shared_ptr<IAtom> { return find(Category::Emitter, id); }
	auto find(const int id, receiver_type) noexcept -> std::shared_ptr<IAtom> { return find(Category::Receiver, id); }

private:
	std::vector<std::shared_ptr<IAtom>> m_objects;
	std::vector<INotification *> m_notifications;
	mutable std::recursive_mutex m_mutex;
};
