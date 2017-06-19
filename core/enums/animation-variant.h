#pragma once

#include <array>
#include <core/types.h>

namespace Enum {

enum class AnimationVariant {
	Rainbow,
	Candle,
	Unused,
};

constexpr std::array<AnimationVariant, static_cast<u32>(AnimationVariant::Unused)> getAnimationVariantTypes() {
	return {{
		AnimationVariant::Rainbow,
		AnimationVariant::Candle,
	}};
}
}
