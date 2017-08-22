#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <core/containers/color-scanline-container.h>

#include <functional>
#include <list>
#include <memory>
#include <vector>
#include <mutex>
#include <experimental/any>

#include <core/generic/iatom.h>
#include <core/generic/inotification.h>

class AtomAggregator {
public:
	void attach(INotification *notificator) noexcept {
		std::lock_guard<std::mutex> _(m_mutex);
		m_notifications.emplace_back(notificator);

		for (const auto &object : m_objects)
			notificator->action(NotifyAction::Attached, object);
	}

	void detach(INotification *object) noexcept {
		std::lock_guard<std::mutex> _(m_mutex);
		m_notifications.erase(std::remove_if(m_notifications.begin(),
								  m_notifications.end(),
								  [object](const auto &value) { return object == value; }),
			m_notifications.end());
	}

	void attach(const std::shared_ptr<IAtom> &object) noexcept {
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

	void detach(const std::shared_ptr<IAtom> &object) noexcept {
		std::lock_guard<std::mutex> _(m_mutex);

		for (auto notification : m_notifications)
			notification->action(NotifyAction::Detached, object);

		m_objects.erase(std::remove_if(m_objects.begin(),
							m_objects.end(),
							[ptr = object.get()](const auto &value) { return ptr == value.get(); }),
			m_objects.end());
	}

	void enumerate(std::function<void(const std::shared_ptr<IAtom> &)> callback) const noexcept {
		std::lock_guard<std::mutex> _(m_mutex);
		for (const auto &object : m_objects)
			callback(object);
	}

private:
	std::vector<std::shared_ptr<IAtom>> m_objects;
	std::vector<INotification *> m_notifications;
	mutable std::mutex m_mutex;
};

namespace Corrector {
namespace Interface {

class ICorrectorNotify;

class ICorrectorManager : public ::Interface::INotify {
public:
	explicit ICorrectorManager()
			: INotify(-1) {}
	~ICorrectorManager() override = default;

	virtual void attach(ICorrectorNotify *notify) = 0;
	virtual void detach(ICorrectorNotify *notify) = 0;

	virtual void attach(const std::shared_ptr<ICorrector> &corrector) = 0;
	virtual void detach(const std::shared_ptr<ICorrector> &corrector) = 0;

	virtual std::shared_ptr<ICorrector> find(const int id) const = 0;
	virtual void enumerate(std::function<void(const std::shared_ptr<ICorrector> &)> callback) const = 0;

	virtual void execute(Container::Scanline &scanline) = 0;
};
}
}
