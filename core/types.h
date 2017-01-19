#pragma once

#include <cstdint>
#include <type_traits>

using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i8 = int8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u8 = uint8_t;

static_assert(std::is_same<i16, int16_t>::value, "");
static_assert(std::is_same<i32, int32_t>::value, "");
static_assert(std::is_same<i64, int64_t>::value, "");
static_assert(std::is_same<i8, int8_t>::value, "");
static_assert(std::is_same<u16, uint16_t>::value, "");
static_assert(std::is_same<u32, uint32_t>::value, "");
static_assert(std::is_same<u64, uint64_t>::value, "");
static_assert(std::is_same<u8, uint8_t>::value, "");

static_assert(sizeof(i16) == 2, "");
static_assert(sizeof(i32) == 4, "");
static_assert(sizeof(i64) == 8, "");
static_assert(sizeof(i8) == 1, "");
static_assert(sizeof(u16) == 2, "");
static_assert(sizeof(u32) == 4, "");
static_assert(sizeof(u64) == 8, "");
static_assert(sizeof(u8) == 1, "");

static_assert(std::is_signed<i16>::value, "");
static_assert(std::is_signed<i32>::value, "");
static_assert(std::is_signed<i64>::value, "");
static_assert(std::is_signed<i8>::value, "");
static_assert(std::is_unsigned<u16>::value, "");
static_assert(std::is_unsigned<u32>::value, "");
static_assert(std::is_unsigned<u64>::value, "");
static_assert(std::is_unsigned<u8>::value, "");

using ci16 = const int16_t;
using ci32 = const int32_t;
using ci64 = const int64_t;
using ci8 = const int8_t;
using cu16 = const uint16_t;
using cu32 = const uint32_t;
using cu64 = const uint64_t;
using cu8 = const uint8_t;

static_assert(std::is_same<cu16, const uint16_t>::value, "");
static_assert(std::is_same<cu32, const uint32_t>::value, "");
static_assert(std::is_same<cu64, const uint64_t>::value, "");
static_assert(std::is_same<cu8, const uint8_t>::value, "");
static_assert(std::is_same<ci16, const int16_t>::value, "");
static_assert(std::is_same<ci32, const int32_t>::value, "");
static_assert(std::is_same<ci64, const int64_t>::value, "");
static_assert(std::is_same<ci8, const int8_t>::value, "");

static_assert(sizeof(cu16) == 2, "");
static_assert(sizeof(cu32) == 4, "");
static_assert(sizeof(cu64) == 8, "");
static_assert(sizeof(cu8) == 1, "");
static_assert(sizeof(ci16) == 2, "");
static_assert(sizeof(ci32) == 4, "");
static_assert(sizeof(ci64) == 8, "");
static_assert(sizeof(ci8) == 1, "");

static_assert(std::is_signed<ci16>::value, "");
static_assert(std::is_signed<ci32>::value, "");
static_assert(std::is_signed<ci64>::value, "");
static_assert(std::is_signed<ci8>::value, "");
static_assert(std::is_unsigned<cu16>::value, "");
static_assert(std::is_unsigned<cu32>::value, "");
static_assert(std::is_unsigned<cu64>::value, "");
static_assert(std::is_unsigned<cu8>::value, "");

using color = u32;
using ccolor = cu32;

using cchar = const char;