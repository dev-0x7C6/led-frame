#include <core/emitters/animation-emitter.h>

#include <QColor>

using namespace Emitter;

AnimationEmitter::AnimationEmitter(QObject *parent)
	: QObject(parent)

{
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, this, &AnimationEmitter::process);
	m_animation.setKeyValues(QVariantAnimation::KeyValues());
	m_animation.setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
	m_animation.setKeyValueAt(0.200, QColor::fromRgbF(0, 0, 1));
	m_animation.setKeyValueAt(0.400, QColor::fromRgbF(0, 1, 1));
	m_animation.setKeyValueAt(0.600, QColor::fromRgbF(0, 1, 0));
	m_animation.setKeyValueAt(0.800, QColor::fromRgbF(1, 1, 0));
	m_animation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
	m_animation.setDuration(5000);
	m_animation.setLoopCount(-1);
	m_animation.start();
	m_colors.fill(0);
}

AnimationEmitter::~AnimationEmitter() {
	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();
}

Enum::EmitterType AnimationEmitter::type() const {
	return Enum::EmitterType::Animation;
}

void AnimationEmitter::onConnect(const uint32_t &count) {
	if (m_animation.state() != QAbstractAnimation::Running && count > 0)
		m_animation.start();
}

void AnimationEmitter::onDisconnect(const uint32_t &count) {
	if (count != 0)
		return;

	if (m_animation.state() == QAbstractAnimation::Running)
		m_animation.stop();
}

void AnimationEmitter::process(const QVariant &value) {
	m_colors.rotate((qvariant_cast<QColor>(value)).rgb());
	commit(m_colors);
}
