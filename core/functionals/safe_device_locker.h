#pragma once

#include <core/functionals/safe_set.h>

namespace safe {

class device_locker {
public:
	bool lock(const std::string &device) {
		if (m_locker.count(device) != 0)
			return false;

		m_locker.emplace(device);
		return true;
	}

	void unlock(const std::string &device) {
		m_locker.erase(device);
	}

private:
	set<std::string> m_locker;
};

} // namespace safe
