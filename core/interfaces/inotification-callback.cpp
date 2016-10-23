#include "inotification-callback.h"

using namespace Interface;

std::atomic<int> INotificationCallback::m_idCounter{0};

INotificationCallback::INotificationCallback()
		: m_id(m_idCounter++) {
}

int INotificationCallback::id() const { return m_id; }
