#pragma once

#include <array>
#include <core/types.h>

namespace Enum {

enum class AnimationVariant {
	Rainbow,
	Candle,
	Unused,
};

inline constexpr std::initializer_list<AnimationVariant> getAnimationVariantTypes() {
	return {
		AnimationVariant::Rainbow,
		AnimationVariant::Candle,
	};
}

} // namespace Enum
