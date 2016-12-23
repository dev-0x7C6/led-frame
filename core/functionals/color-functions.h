#pragma once

#include <core/types.h>
#include <algorithm>

namespace Functional {
namespace Color {

constexpr auto R_CHANNEL_SHIFT = 0x10u;
constexpr auto G_CHANNEL_SHIFT = 0x8u;

constexpr auto RGB_MAX_VALUE = 0xffu;
constexpr auto R_CHANNEL_MASK = RGB_MAX_VALUE << R_CHANNEL_SHIFT;
constexpr auto G_CHANNEL_MASK = RGB_MAX_VALUE << G_CHANNEL_SHIFT;
constexpr auto B_CHANNEL_MASK = RGB_MAX_VALUE;

inline color getR(ccolor rgb) { return (rgb >> R_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
inline color getG(ccolor rgb) { return (rgb >> G_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
inline color getB(ccolor rgb) { return rgb & RGB_MAX_VALUE; }
inline color putR(ccolor r) { return (std::min(RGB_MAX_VALUE, r) << R_CHANNEL_SHIFT); }
inline color putG(ccolor g) { return (std::min(RGB_MAX_VALUE, g) << G_CHANNEL_SHIFT); }
inline color putB(ccolor b) { return std::min(RGB_MAX_VALUE, b); }
inline color overwriteR(ccolor value, ccolor r) { return (value & ~R_CHANNEL_MASK) | putR(std::min(RGB_MAX_VALUE, r)); }
inline color overwriteG(ccolor value, ccolor g) { return (value & ~G_CHANNEL_MASK) | putG(std::min(RGB_MAX_VALUE, g)); }
inline color overwriteB(ccolor value, ccolor b) { return (value & ~B_CHANNEL_MASK) | putB(std::min(RGB_MAX_VALUE, b)); }
inline color rgb(ccolor r, ccolor g, ccolor b) { return putR(r) | putG(g) | putB(b); }
}
}
