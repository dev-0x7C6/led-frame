#include "animation-variant-factory.h"

#include <QVariantAnimation>
#include <QColor>

using namespace Enum;
using namespace Functional::Animation;

void AnimationVariantFactory::setup(QVariantAnimation &animation, const AnimationVariant &type) noexcept {
	animation.setKeyValues({});

	switch (type) {
		case AnimationVariant::Rainbow:
			for (int i = 0; i < 359; ++i)
				animation.setKeyValueAt(static_cast<double>(i) / 359.0, QColor::fromHsv(i, 255, 255, 255));
			break;

		case AnimationVariant::Candle:
			animation.setKeyValueAt(0.000, QColor::fromHsv(15, 255, 255, 255));
			animation.setKeyValueAt(0.100, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.150, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.200, QColor::fromHsv(15, 255, 255, 255));
			animation.setKeyValueAt(0.250, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.275, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.300, QColor::fromHsv(18, 255, 255, 255));
			animation.setKeyValueAt(0.400, QColor::fromHsv(15, 255, 255, 255));
			animation.setKeyValueAt(0.500, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.550, QColor::fromHsv(15, 255, 255, 255));
			animation.setKeyValueAt(0.600, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.625, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.700, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(0.800, QColor::fromHsv(15, 255, 255, 255));
			animation.setKeyValueAt(0.850, QColor::fromHsv(25, 255, 255, 255));
			animation.setKeyValueAt(0.900, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.925, QColor::fromHsv(18, 255, 255, 255));
			animation.setKeyValueAt(0.950, QColor::fromHsv(10, 255, 255, 255));
			animation.setKeyValueAt(0.975, QColor::fromHsv(20, 255, 255, 255));
			animation.setKeyValueAt(1.000, QColor::fromHsv(15, 255, 255, 255));
			break;

		case AnimationVariant::Unused:
			break;
	}

	animation.setDuration(50000);
	animation.setLoopCount(-1);
	animation.start();
}
