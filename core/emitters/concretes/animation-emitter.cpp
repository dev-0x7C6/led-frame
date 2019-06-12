#include <core/emitters/concretes/animation-emitter.h>
#include <core/functionals/animations/animation-variant-factory.h>
#include <chrono>

#include <iostream>

using namespace Emitter::Concrete;
using namespace Functional::Animation;

AnimationEmitter::AnimationEmitter() {
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, [this](const auto &value) { process(value); });
	make_animation(m_animation, Enum::AnimationVariant::Candle);
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();

	QObject::disconnect(&m_animation, nullptr, nullptr, nullptr);
}

void AnimationEmitter::interpret(std::any data) noexcept {
	make_animation(m_animation, std::any_cast<Enum::AnimationVariant>(data));
}

void AnimationEmitter::process(const QVariant &value) {
	commit(Container::Scanline(qvariant_cast<QColor>(value).rgb()));
}
