#pragma once

#include <core/enums/animation-variant.h>

class QVariantAnimation;

namespace Functional {
namespace Animation {

class AnimationVariantFactory {
public:
	explicit AnimationVariantFactory() = delete;
	virtual ~AnimationVariantFactory() = delete;

	static void setup(QVariantAnimation &animation, const Enum::AnimationVariant &type) noexcept;
};
}
}
