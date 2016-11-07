#pragma once

#include <cstdint>

namespace Functional {
namespace Color {

constexpr auto R_CHANNEL_SHIFT = 0x10u;
constexpr auto G_CHANNEL_SHIFT = 0x8u;
constexpr auto B_CHANNEL_SHIFT = 0x0u;

constexpr auto RGB_MAX_VALUE = 0xffu;
constexpr auto R_CHANNEL_MASK = RGB_MAX_VALUE << R_CHANNEL_SHIFT;
constexpr auto G_CHANNEL_MASK = RGB_MAX_VALUE << G_CHANNEL_SHIFT;
constexpr auto B_CHANNEL_MASK = RGB_MAX_VALUE << B_CHANNEL_SHIFT;

constexpr inline uint32_t getR(const uint32_t rgb) { return (rgb >> R_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
constexpr inline uint32_t getG(const uint32_t rgb) { return (rgb >> G_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
constexpr inline uint32_t getB(const uint32_t rgb) { return (rgb >> B_CHANNEL_SHIFT) & RGB_MAX_VALUE; }
constexpr inline uint32_t putR(const uint32_t r) { return (std::min(RGB_MAX_VALUE, r) << R_CHANNEL_SHIFT); }
constexpr inline uint32_t putG(const uint32_t g) { return (std::min(RGB_MAX_VALUE, g) << G_CHANNEL_SHIFT); }
constexpr inline uint32_t putB(const uint32_t b) { return (std::min(RGB_MAX_VALUE, b) << B_CHANNEL_SHIFT); }
constexpr inline uint32_t overwriteR(const uint32_t color, const uint32_t r) { return (color & ~R_CHANNEL_MASK) | putR(std::min(RGB_MAX_VALUE, r)); }
constexpr inline uint32_t overwriteG(const uint32_t color, const uint32_t g) { return (color & ~G_CHANNEL_MASK) | putG(std::min(RGB_MAX_VALUE, g)); }
constexpr inline uint32_t overwriteB(const uint32_t color, const uint32_t b) { return (color & ~B_CHANNEL_MASK) | putB(std::min(RGB_MAX_VALUE, b)); }
constexpr inline uint32_t rgb(const uint32_t r, const uint32_t g, const uint32_t b) { return putR(r) | putG(g) | putB(b); }
}
}
