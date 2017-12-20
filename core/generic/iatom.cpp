#include "iatom.h"

IAtom::IAtom(const int id)
		: m_id(id) {
}

auto IAtom::id() const noexcept -> int {
	return m_id;
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

auto IAtom::properties() const noexcept -> Properties { return {}; }
