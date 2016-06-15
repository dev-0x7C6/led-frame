#pragma once

#include <QObject>
#include <functional>

namespace Interface {

class INotificationCallback : public QObject {
	Q_OBJECT
public:
	explicit INotificationCallback() = default;
	virtual ~INotificationCallback() = default;

	void callback(QObject *object, const std::function<void()> &callback) {
		connect(this, &INotificationCallback::notify, object, callback);
	}

signals:
	void notify();
};
}
