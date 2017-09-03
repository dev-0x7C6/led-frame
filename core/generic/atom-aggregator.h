#pragma once

#include <core/generic/iatom.h>
#include <core/generic/inotification.h>

#include <mutex>

class AtomAggregator {
public:
	void attach(INotification *notificator) noexcept;
	void detach(INotification *notificator) noexcept;

	void attach(const std::shared_ptr<IAtom> &object) noexcept;
	void detach(const std::shared_ptr<IAtom> &object) noexcept;

	void enumerate(std::function<void(const std::shared_ptr<IAtom> &)> callback) const noexcept;
	auto find(const Category category, const int id) noexcept -> std::shared_ptr<IAtom>;

private:
	std::vector<std::shared_ptr<IAtom>> m_objects;
	std::vector<INotification *> m_notifications;
	mutable std::mutex m_mutex;
};
