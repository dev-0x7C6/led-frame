#include "iatom.h"

IAtom::IAtom(const int id)
		: m_id(id) {
}

auto IAtom::category() const noexcept -> Category {
	return Category::Undefined;
}

void IAtom::attach(std::function<void()> callback) {
	m_callbackConnector.attach(callback);
}

void IAtom::notify2() {
	m_callbackConnector.call();
}

std::vector<std::pair<std::string, std::experimental::any>> IAtom::properties() const noexcept {
	return {};
}

auto IAtom::sharedFromThis() noexcept { return shared_from_this(); }
