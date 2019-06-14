#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <variant>

#include <core/enums/ecategory.h>
#include <externals/common/types.hpp>

using Property = std::variant<std::string, double, int, bool>;
using Properties = std::vector<std::pair<std::string, Property>>;

i32 get_unique_i32();

class CallbackStorage {
public:
	void attach(std::function<void()> &&callback);
	void notify();

private:
	std::vector<std::function<void()>> m_callbacks;
};

class IRepresentable : public std::enable_shared_from_this<IRepresentable>, public CallbackStorage {
public:
	IRepresentable();
	virtual ~IRepresentable();

	IRepresentable(const IRepresentable &) = delete;
	IRepresentable(IRepresentable &&) = delete;
	IRepresentable &operator=(const IRepresentable &) = delete;
	IRepresentable &operator=(IRepresentable &&) = delete;

	auto id() const noexcept { return m_id; }
	virtual auto category() const noexcept -> Category { return Category::Undefined; }
	virtual auto properties() const noexcept -> Properties { return {}; }

private:
	const i32 m_id{};
};
