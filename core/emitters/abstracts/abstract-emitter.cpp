#include <core/emitters/abstracts/abstract-emitter.h>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Emitter::Abstract;

AbstractEmitter::AbstractEmitter(cint id)
		: IEmitter(id) {
}

QString AbstractEmitter::name() const {
	return m_name;
}

void AbstractEmitter::setName(const QString &name) {
	m_name = name;
}
