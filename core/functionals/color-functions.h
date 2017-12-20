#pragma once

#include <core/types.h>
#include <algorithm>

namespace Functional {

constexpr auto R_CHANNEL_SHIFT = 0x10u;
constexpr auto G_CHANNEL_SHIFT = 0x8u;

constexpr auto RGB_MAX_VALUE = 0xffu;
constexpr auto R_CHANNEL_MASK = RGB_MAX_VALUE << R_CHANNEL_SHIFT;
constexpr auto G_CHANNEL_MASK = RGB_MAX_VALUE << G_CHANNEL_SHIFT;
constexpr auto B_CHANNEL_MASK = RGB_MAX_VALUE;

constexpr color get_r24(ccolor rgb) { return (rgb >> R_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
constexpr color get_g24(ccolor rgb) { return (rgb >> G_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
constexpr color get_b24(ccolor rgb) { return rgb & RGB_MAX_VALUE; }
constexpr color put_r24(ccolor r) { return (std::min(RGB_MAX_VALUE, r) << R_CHANNEL_SHIFT); }
constexpr color put_g24(ccolor g) { return (std::min(RGB_MAX_VALUE, g) << G_CHANNEL_SHIFT); }
constexpr color put_b24(ccolor b) { return std::min(RGB_MAX_VALUE, b); }
constexpr color rgb(ccolor r, ccolor g, ccolor b) { return put_r24(r) | put_g24(g) | put_b24(b); }
}
