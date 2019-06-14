#include "irepresentable.h"

#include <atomic>
#include <externals/common/logger/logger.hpp>

namespace {
constexpr auto filter = error_class::information;
}

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

IRepresentable::IRepresentable()
		: m_id(get_unique_i32()) {
	logger<filter>::debug(__PRETTY_FUNCTION__, ", id: ", m_id);
}

IRepresentable::~IRepresentable() {
	logger<filter>::debug(__PRETTY_FUNCTION__, ", id: ", m_id);
}
