#include <core/emitters/concretes/animation-emitter.h>

#include <QColor>

using namespace Emitter::Concrete;

AnimationEmitter::AnimationEmitter(int id)
		: AbstractEmitter(id)

{
	const auto start = QColor::fromRgbF(1, 0, 0);
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, this, &AnimationEmitter::process);
	m_animation.setKeyValues(QVariantAnimation::KeyValues());
	m_animation.setKeyValueAt(0.000, start);
	m_animation.setKeyValueAt(0.200, QColor::fromRgbF(0, 0, 1));
	m_animation.setKeyValueAt(0.400, QColor::fromRgbF(0, 1, 1));
	m_animation.setKeyValueAt(0.600, QColor::fromRgbF(0, 1, 0));
	m_animation.setKeyValueAt(0.800, QColor::fromRgbF(1, 1, 0));
	m_animation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
	m_animation.setDuration(50000);
	m_animation.setLoopCount(-1);
	m_animation.start();
	m_colors.fill(start.rgba());
	commit(m_colors);
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();
}

Enum::EmitterType AnimationEmitter::type() const {
	return Enum::EmitterType::Animation;
}

void AnimationEmitter::process(const QVariant &value) {
	m_colors = qvariant_cast<QColor>(value).rgb();
	commit(m_colors);
}
