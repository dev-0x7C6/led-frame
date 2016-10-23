#pragma once

#include <atomic>
#include <functional>

#include <QObject>

namespace Interface {

class INotificationCallback : public QObject {
	Q_OBJECT
public:
	explicit INotificationCallback();
	virtual ~INotificationCallback() = default;

	void callback(QObject *object, const std::function<void()> &callback) {
		connect(this, &INotificationCallback::notify, object, callback);
	}

	int id() const;

private:
	static std::atomic<int> m_idCounter;
	const std::atomic<int> m_id;

signals:
	void notify();
};
}
