#pragma once

#include <array>
#include <core/types.h>

namespace Enum {

enum class AnimationVariant {
	Rainbow,
	Candle,
	Unused,
};

constexpr static auto animationVariantTypes = std::initializer_list<AnimationVariant>{
	AnimationVariant::Rainbow,
	AnimationVariant::Candle,
};

} // namespace Enum
