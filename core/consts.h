#pragma once

#include <cstdint>
#include <type_traits>

using cuint16_t = const uint16_t;
using cuint32_t = const uint32_t;
using cuint64_t = const uint64_t;
using cuint8_t = const uint8_t;

static_assert(std::is_const<cuint16_t>::value, "should be constant");
static_assert(std::is_const<cuint32_t>::value, "should be constant");
static_assert(std::is_const<cuint64_t>::value, "should be constant");
static_assert(std::is_const<cuint8_t>::value, "should be constant");

using cint16_t = const int16_t;
using cint32_t = const int32_t;
using cint64_t = const int64_t;
using cint8_t = const int8_t;

static_assert(std::is_const<cint16_t>::value, "should be constant");
static_assert(std::is_const<cint32_t>::value, "should be constant");
static_assert(std::is_const<cint64_t>::value, "should be constant");
static_assert(std::is_const<cint8_t>::value, "should be constant");

using cint = cint32_t;
using cuint = cuint32_t;

static_assert(std::is_const<cint>::value, "should be constant");
static_assert(std::is_const<cuint>::value, "should be constant");

using color = uint32_t;
using ccolor = cuint32_t;
