#include "irepresentable.h"

#include <atomic>

void CallbackStorage::attach(std::function<void()> &&callback) {
	m_callbacks.emplace_back(std::move(callback));
}

void CallbackStorage::notify() {
	for (auto &&callback : m_callbacks)
		callback();
}

i32 get_unique_i32() {
	static std::atomic<i32> id{};
	return id++;
}
