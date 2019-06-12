#include <core/emitters/abstracts/abstract-emitter.h>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Emitter::Abstract;

std::string AbstractEmitter::name() const {
	return m_name;
}

void AbstractEmitter::setName(const std::string &name) {
	m_name = name;
}
