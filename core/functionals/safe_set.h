#pragma once

#include <mutex>
#include <set>

namespace safe {
template <typename type>
class set {
public:
	template <typename... types>
	void emplace(types &&... values) {
		std::lock_guard _(m_mutex);
		m_set.emplace(std::forward<types>(values)...);
	}

	template <typename... types>
	decltype(auto) count(types &&... values) const noexcept {
		std::lock_guard _(m_mutex);
		return m_set.count(std::forward<types>(values)...);
	}

	template <typename... types>
	void erase(types &&... values) {
		std::lock_guard _(m_mutex);
		m_set.erase(std::forward<types>(values)...);
	}

private:
	std::set<type> m_set;
	mutable std::mutex m_mutex;
};
} // namespace safe
