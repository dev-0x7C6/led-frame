#pragma once

#include <experimental/any>
#include <functional>
#include <memory>
#include <vector>

enum class Category {
	Emitter,
	Receiver,
	Corrector,
	Undefined,
};

class IAtom : public std::enable_shared_from_this<IAtom> {
public:
	explicit IAtom(const int id);
	virtual ~IAtom() = default;

	virtual auto category() const noexcept -> Category;

	void attach(std::function<void()> callback);
	void notify();

	virtual std::vector<std::pair<std::string, std::experimental::any>> properties() const noexcept;

	auto sharedFromThis() noexcept;

	auto id() const noexcept -> int { return m_id; }

private:
	const int m_id = 0;
	std::vector<std::function<void()>> m_callbacks;
};

constexpr auto toString(const Category &source) noexcept {
	switch (source) {
		case Category::Corrector:
			return "corrector";
		case Category::Emitter:
			return "emitter";
		case Category::Receiver:
			return "receiver";
		case Category::Undefined:
			return "undefined";
	}

	return "undefined";
}
