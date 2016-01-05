#include <core/emitters/animation-emitter.h>

#include <QColor>

using namespace Emitter;

AnimationEmitter::AnimationEmitter(QObject *parent)
	: QObject(parent)

{
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, this, &AnimationEmitter::process);
	m_animation.setKeyValues(QVariantAnimation::KeyValues());
	m_animation.setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
	m_animation.setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
	m_animation.setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
	m_animation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
	m_animation.setDuration(10000);
	m_animation.setLoopCount(-1);
	m_animation.start();
}

QString AnimationEmitter::name() const {
	return "Animation";
}

Enum::EmitterType AnimationEmitter::type() const {
	return Enum::EmitterType::Animation;
}

void AnimationEmitter::process(const QVariant &value) {
	m_colors.fill((qvariant_cast<QColor>(value)).rgb());
	commit(m_colors);
}
