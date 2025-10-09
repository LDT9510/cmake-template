#pragma once

#include <cstdint>

// ReSharper disable CppInconsistentNaming
using b8 = bool;

using u8 = std::uint_least8_t;
using u16 = std::uint_least16_t;
using u32 = std::uint_least32_t;
using u64 = std::uint_least64_t;

using i8 = std::int_least8_t;
using i16 = std::int_least16_t;
using i32 = std::int_least32_t;
using i64 = std::int_least64_t;

using f32 = float;
using f64 = double;
// ReSharper restore CppInconsistentNaming

// these should be revised for specifics platforms
static_assert(sizeof(b8) == 1, "Bad boolean size for current platform");
static_assert(sizeof(f32) == 4, "Bad float size for current platform");
static_assert(sizeof(f64) == 8, "Bad double size for current platform");
