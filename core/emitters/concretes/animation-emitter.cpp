#include <core/emitters/concretes/animation-emitter.h>
#include <core/functionals/animations/animation-variant-factory.h>
#include <chrono>

#include <iostream>

using namespace Emitter::Concrete;
using namespace Functional::Animation;

AnimationEmitter::AnimationEmitter(ci32 id)
		: AbstractEmitter(id)

{
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, this, &AnimationEmitter::process);
	AnimationVariantFactory::setup(m_animation, Enum::AnimationVariant::Candle);
	m_colors.fill(m_animation.keyValueAt(0).value<QColor>().rgba());
	commit(m_colors);
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();
}

Enum::EmitterType AnimationEmitter::type() const {
	return Enum::EmitterType::Animation;
}

void AnimationEmitter::interpret(any data) noexcept {
	AnimationVariantFactory::setup(m_animation, any_cast<Enum::AnimationVariant>(data));
}

void AnimationEmitter::process(const QVariant &value) {
	m_colors = qvariant_cast<QColor>(value).rgb();
	commit(m_colors);
}
