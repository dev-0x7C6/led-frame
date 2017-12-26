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

template <typename type>
constexpr decltype(auto) get_r24(const type &rgb) { return (rgb >> R_CHANNEL_SHIFT) & RGB_MAX_VALUE; }

template <typename type>
constexpr decltype(auto) get_g24(const type &rgb) { return (rgb >> G_CHANNEL_SHIFT) & RGB_MAX_VALUE; }

template <typename type>
constexpr decltype(auto) get_b24(const type &rgb) { return rgb & RGB_MAX_VALUE; }

template <typename type>
constexpr decltype(auto) put_r24(const type &r) { return (std::min(RGB_MAX_VALUE, r) << R_CHANNEL_SHIFT); }

template <typename type>
constexpr decltype(auto) put_g24(const type &g) { return (std::min(RGB_MAX_VALUE, g) << G_CHANNEL_SHIFT); }

template <typename type>
constexpr decltype(auto) put_b24(const type &b) { return std::min(RGB_MAX_VALUE, b); }

template <typename type>
constexpr decltype(auto) rgb(const type &r, const type &g, const type &b) { return put_r24(r) | put_g24(g) | put_b24(b); }
} // namespace Functional
