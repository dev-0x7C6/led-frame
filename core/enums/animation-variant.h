#pragma once

#include <array>
#include <core/types.h>

namespace Enum {

enum class AnimationVariant {
	Rainbow,
	Candle,
	Unused,
};

template <typename type = std::array<AnimationVariant, static_cast<u32>(AnimationVariant::Unused)>>
constexpr type getAnimationVariantTypes() {
	return {{
		AnimationVariant::Rainbow,
		AnimationVariant::Candle,
	}};
}
} // namespace Enum
