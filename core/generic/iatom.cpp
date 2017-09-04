#include "iatom.h"

IAtom::IAtom(const int id)
		: m_id(id) {
}

auto IAtom::category() const noexcept -> Category {
	return Category::Undefined;
}

void IAtom::attach(std::function<void()> callback) {
	m_callbacks.push_back(callback);
}

void IAtom::notify() {
	for (auto &callback : m_callbacks)
		callback();
}

std::vector<std::pair<std::string, std::experimental::any>> IAtom::properties() const noexcept {
	return {};
}

auto IAtom::sharedFromThis() noexcept { return shared_from_this(); }
