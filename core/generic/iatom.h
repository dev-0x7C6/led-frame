#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <experimental/any>

template <typename functor = std::function<void()>>
class CallbackConnector {
public:
	~CallbackConnector() = default;

	void attach(functor func) {
		m_callbacks.push_back(func);
	}

	void call() {
		for (auto &callback : m_callbacks)
			callback();
	}

private:
	std::vector<functor> m_callbacks;
};

enum class Category {
	Emitter,
	Receiver,
	Corrector
};

constexpr auto toString(const Category &source) noexcept {
	switch (source) {
		case Category::Corrector:
			return "corrector";
		case Category::Emitter:
			return "emitter";
		case Category::Receiver:
			return "receiver";
	}

	return "";
}

class IAtom : public std::enable_shared_from_this<IAtom> {
public:
	virtual ~IAtom() = default;

	virtual auto category() const noexcept -> Category = 0;

	void attach(std::function<void()> callback);
	void notify2();

	virtual std::vector<std::pair<std::string, std::experimental::any>> properties() const noexcept;

	auto sharedFromThis() noexcept;

private:
	CallbackConnector<std::function<void()>> m_callbackConnector;
};
