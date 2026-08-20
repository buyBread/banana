#pragma once

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8  = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using f32 = float;
using f64 = double;

using c8  = char8_t;
using c16 = char16_t;
using c32 = char32_t;

/*
    compile correctly
*/

static_assert(sizeof(u8)  == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

static_assert(sizeof(i8)  == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

static_assert(sizeof(c8)  == 1);
static_assert(sizeof(c16) == 2);
static_assert(sizeof(c32) == 4);

/*
    bounds
*/

#define U8_MIN  0x00
#define U8_MAX  0xFF
#define U16_MIN 0x0000
#define U16_MAX 0xFFFF
#define U32_MIN 0x00000000
#define U32_MAX 0xFFFFFFFF
#define U64_MIN 0x0000000000000000ull
#define U64_MAX 0xFFFFFFFFFFFFFFFFull

#define I8_MIN  (-0x7F - 1)
#define I8_MAX  0x7F
#define I16_MIN (-0x7FFF - 1)
#define I16_MAX 0x7FFF
#define I32_MIN (-0x7FFFFFFF - 1)
#define I32_MAX 0x7FFFFFFF
#define I64_MIN (-0x7FFFFFFFFFFFFFFFll - 1)
#define I64_MAX 0x7FFFFFFFFFFFFFFFll

#define F32_MIN     1.175494351e-38F
#define F32_MAX     3.402823466e+38F
#define F32_LOWEST  (-F32_MAX)
#define F32_EPSILON 1.192092896e-07F

#define F64_MIN     2.2250738585072014e-308
#define F64_MAX     1.7976931348623158e+308
#define F64_LOWEST  (-F64_MAX)
#define F64_EPSILON 2.2204460492503131e-16