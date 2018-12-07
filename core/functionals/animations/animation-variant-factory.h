#pragma once

#include <core/enums/animation-variant.h>

class QVariantAnimation;

namespace Functional {
namespace Animation {

void make_animation(QVariantAnimation &animation, Enum::AnimationVariant type) noexcept;

} // namespace Animation
} // namespace Functional
