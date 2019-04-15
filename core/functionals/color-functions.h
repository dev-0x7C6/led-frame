#pragma once

#include <core/types.h>
#include <algorithm>

namespace Functional {

constexpr auto R_CHANNEL_SHIFT = 16;
constexpr auto G_CHANNEL_SHIFT = 8;

constexpr auto RGB_MAX_VALUE = 0xffu;
constexpr auto R_CHANNEL_MASK = RGB_MAX_VALUE << R_CHANNEL_SHIFT;
constexpr auto G_CHANNEL_MASK = RGB_MAX_VALUE << G_CHANNEL_SHIFT;
constexpr auto B_CHANNEL_MASK = RGB_MAX_VALUE;

template <typename type>
constexpr u32 get_r24(type &&rgb) { return (rgb >> R_CHANNEL_SHIFT) & RGB_MAX_VALUE; }

template <typename type>
constexpr u32 get_g24(type &&rgb) { return (rgb >> G_CHANNEL_SHIFT) & RGB_MAX_VALUE; }

template <typename type>
constexpr u32 get_b24(type &&rgb) { return rgb & RGB_MAX_VALUE; }

template <typename type>
constexpr u32 put_r24(type &&r) { return r << R_CHANNEL_SHIFT; }

template <typename type>
constexpr u32 put_g24(type &&g) { return g << G_CHANNEL_SHIFT; }

template <typename type>
constexpr u32 put_b24(type &&b) { return b; }

template <typename type>
constexpr u32 rgb(type &&r, type &&g, type &&b) { return put_r24(r) | put_g24(g) | put_b24(b); }
} // namespace Functional
