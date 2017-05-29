#include <core/emitters/concretes/animation-emitter.h>

#include <QColor>

#include <chrono>

using namespace Emitter::Concrete;

enum class AnimationType {
	Rainbow,
	Candle,
};

class AnimationFactory {
public:
	explicit AnimationFactory() = delete;
	virtual ~AnimationFactory() = delete;

	static void setup(QVariantAnimation &animation, const AnimationType &type) noexcept;
};

void AnimationFactory::setup(QVariantAnimation &animation, const AnimationType &type) noexcept {
	animation.setKeyValues({});

	switch (type) {
		case AnimationType::Rainbow:
			for (int i = 0; i < 359; ++i)
				animation.setKeyValueAt(static_cast<double>(i) / 359.0, QColor::fromHsv(i, 255, 255, 255));
			break;

		case AnimationType::Candle:
			animation.setKeyValueAt(0.000, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.100, QColor::fromHsv(30, 255, 255, 255));
			animation.setKeyValueAt(0.150, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.200, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.250, QColor::fromHsv(35, 255, 255, 255));
			animation.setKeyValueAt(0.275, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.300, QColor::fromHsv(28, 255, 255, 255));
			animation.setKeyValueAt(0.400, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.500, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.550, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.600, QColor::fromHsv(30, 255, 255, 255));
			animation.setKeyValueAt(0.625, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.700, QColor::fromHsv(30, 255, 255, 255));
			animation.setKeyValueAt(0.800, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.850, QColor::fromHsv(30, 255, 255, 255));
			animation.setKeyValueAt(0.900, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.925, QColor::fromHsv(28, 255, 255, 255));
			animation.setKeyValueAt(0.950, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.975, QColor::fromHsv(30, 255, 255, 255));
			animation.setKeyValueAt(1.000, QColor::fromHsv(25, 255, 255, 255));
			break;
	}

	animation.setDuration(50000);
	animation.setLoopCount(-1);
	animation.start();
}

AnimationEmitter::AnimationEmitter(ci32 id)
		: AbstractEmitter(id)

{
	QObject::connect(&m_animation, &QVariantAnimation::valueChanged, this, &AnimationEmitter::process);
	AnimationFactory::setup(m_animation, AnimationType::Candle);
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

void AnimationEmitter::process(const QVariant &value) {
	m_colors = qvariant_cast<QColor>(value).rgb();
	commit(m_colors);
}
