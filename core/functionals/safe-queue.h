#pragma once

#include <mutex>
#include <queue>

namespace safe {

template <typename type>
class queue {
public:
	template <typename... types>
	void emplace(types &&... values) {
		std::lock_guard _(m_mutex);
		m_queue.emplace(std::forward<types>(values)...);
	}

	const type &front() const noexcept {
		std::lock_guard _(m_mutex);
		return m_queue.front();
	}

	void pop() {
		std::lock_guard _(m_mutex);
		m_queue.pop();
	}

	void dequeue_all(std::function<void(const type &)> &&callable) {
		std::lock_guard _(m_mutex);
		while (!m_queue.empty()) {
			callable(m_queue.front());
			m_queue.pop();
		}
	}

private:
	std::queue<type> m_queue;
	mutable std::mutex m_mutex;
};

} // namespace safe
