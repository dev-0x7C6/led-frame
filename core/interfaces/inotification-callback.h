#pragma once

#include <functional>

namespace Interface {

class INotificationCallback {
public:
	explicit INotificationCallback() = default;
	virtual ~INotificationCallback() = default;

	virtual void setNotificationCallback(const std::function<void()> &callback) {
		m_callback = callback;
	}

protected:
	virtual void notify() {
		if (m_callback)
			m_callback();
	}

private:
	std::function<void()> m_callback;
};
}
