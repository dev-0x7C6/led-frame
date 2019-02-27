#include "iatom.h"

IAtom::IAtom(const i32 id)
		: m_id(id) {
}

IAtom::~IAtom() = default;

void IAtom::attach(std::function<void()> &&callback) {
	m_callbacks.emplace_back(std::move(callback));
}

void IAtom::notify() {
	for (const auto &callback : m_callbacks)
		callback();
}
