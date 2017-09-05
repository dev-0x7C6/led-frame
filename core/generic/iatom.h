#pragma once

#include <experimental/any>
#include <functional>
#include <memory>
#include <vector>

#include <core/enums/ecategory.h>

class IAtom : public std::enable_shared_from_this<IAtom> {
public:
	explicit IAtom(const int id);
	virtual ~IAtom() = default;

	auto id() const noexcept -> int;

	virtual auto category() const noexcept -> Category;
	virtual auto properties() const noexcept -> std::vector<std::pair<std::string, std::experimental::any>>;

	void attach(std::function<void()> callback);
	void notify();

private:
	const int m_id = 0;
	std::vector<std::function<void()>> m_callbacks;
};
