#include <core/emitters/concretes/animation-emitter.h>
#include <core/functionals/animations/animation-variant-factory.h>
#include <chrono>

#include <iostream>

using namespace Emitter::Concrete;
using namespace Functional::Animation;

AnimationEmitter::AnimationEmitter(std::any &&args) {
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, [this, firstFrame{true}](const auto &value) mutable {
		if (usages() || firstFrame)
			process(value);

		firstFrame = false;
	});
	make_animation(m_animation, std::any_cast<Enum::AnimationVariant>(args));
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();

	QObject::disconnect(&m_animation, nullptr, nullptr, nullptr);
}

void AnimationEmitter::process(const QVariant &value) {
	if (!value.isValid())
		return;

	const auto color = qvariant_cast<QColor>(value);

	if (color.isValid())
		commit(Container::Scanline(color.rgb()));
}
