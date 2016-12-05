#pragma once

#include <cstdint>
#include <type_traits>

using cuint16 = const uint16_t;
using cuint32 = const uint32_t;
using cuint64 = const uint64_t;
using cuint8 = const uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using uint8 = uint8_t;

static_assert(std::is_const<cuint16>::value, "should be constant");
static_assert(std::is_const<cuint32>::value, "should be constant");
static_assert(std::is_const<cuint64>::value, "should be constant");
static_assert(std::is_const<cuint8>::value, "should be constant");

using cint16 = const int16_t;
using cint32 = const int32_t;
using cint64 = const int64_t;
using cint8 = const int8_t;

static_assert(std::is_const<cint16>::value, "should be constant");
static_assert(std::is_const<cint32>::value, "should be constant");
static_assert(std::is_const<cint64>::value, "should be constant");
static_assert(std::is_const<cint8>::value, "should be constant");

using cint = cint32;
using cuint = cuint32;

static_assert(std::is_const<cint>::value, "should be constant");
static_assert(std::is_const<cuint>::value, "should be constant");

using color = uint32_t;
using ccolor = cuint32;

using cchar = const char;
