#include <core/emitters/concretes/animation-emitter.h>
#include <core/functionals/animations/animation-variant-factory.h>
#include <chrono>

#include <iostream>

using namespace Emitter::Concrete;
using namespace Functional::Animation;

AnimationEmitter::AnimationEmitter(ci32 id)
		: AbstractEmitter(id)

{
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, [this](const auto &value) { process(value); });
	AnimationVariantFactory::setup(m_animation, Enum::AnimationVariant::Candle);
	m_colors.fill(m_animation.keyValueAt(0).value<QColor>().rgba());
	commit(m_colors);
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();

	QObject::disconnect(&m_animation, nullptr, nullptr, nullptr);
}

void AnimationEmitter::interpret(std::any data) noexcept {
	AnimationVariantFactory::setup(m_animation, std::any_cast<Enum::AnimationVariant>(data));
}

void AnimationEmitter::process(const QVariant &value) {
	m_colors = qvariant_cast<QColor>(value).rgb();
	commit(m_colors);
}
