#include "iatom.h"

IAtom::IAtom(std::size_t id)
		: m_id(id) {
}

void IAtom::attach(std::function<void()> &&callback) {
	m_callbacks.emplace_back(std::move(callback));
}

void IAtom::notify() {
	for (const auto &callback : m_callbacks)
		callback();
}
