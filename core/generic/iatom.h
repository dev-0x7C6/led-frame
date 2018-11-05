#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <variant>

#include <core/enums/ecategory.h>

using Property = std::variant<std::string, double, int, bool>;
using Properties = std::vector<std::pair<std::string, Property>>;

class IAtom : public std::enable_shared_from_this<IAtom> {
public:
	explicit IAtom(std::size_t id);
	IAtom(const IAtom &) = delete;
	IAtom(IAtom &&) = delete;
	IAtom &operator=(const IAtom &) = delete;
	IAtom &operator=(IAtom &&) = delete;

	virtual ~IAtom() = default;

	auto id() const noexcept -> int { return static_cast<int>(m_id); }

	virtual auto category() const noexcept -> Category { return Category::Undefined; }
	virtual auto properties() const noexcept -> Properties { return {}; }

	void attach(std::function<void()> &&callback);
	void notify();

private:
	std::vector<std::function<void()>> m_callbacks;
	const std::size_t m_id{};
};
