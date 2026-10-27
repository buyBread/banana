#pragma once

#include <cmath>

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace math {
    template<bool cond, typename T1, typename T2> class if_type;

    template<typename T1, typename T2>
    class if_type<true, T1, T2> {

    public:
        typedef T1 type;

        static const T1 &select(const T1 &a, const T2&) { return a; }
    };

    template<typename T1, typename T2>
    class if_type<false, T1, T2> {

    public:
        typedef T2 type;

        static const T2 &select(const T1&, const T2 &b) { return b; }
    };

    template<bool cond, typename T1, typename T2>
    const typename if_type<cond, T1, T2>::type &compile_time_if(const T1 &a, const T2 &b) {
        return if_type<cond, T1, T2>::select(a, b);
    }

    template<typename A, typename B, typename T1, typename T2>
    class if_type_eq {

    public:
        typedef T2 type;
    };

    template<typename A, typename T1, typename T2>
    class if_type_eq<A, A, T1, T2> {

    public:
        typedef T1 type;
    };

    template<i32 n, typename T> class get_set;

    template<typename T>
    class get_set<0, T> {

    public:
        typedef typename T::get_x_type type;

        static type get(const T &a) { return a.get_x(); }

        template<typename V>
        static void set(T &a, const V &v) { a.set_x(v); }
    };

    template<typename T>
    class get_set<1, T> {

    public:
        typedef typename T::get_y_type type;

        static type get(const T &a) { return a.get_y(); }

        template<typename V>
        static void set(T &a, const V &v) { a.set_y(v); }
    };

    template<typename T>
    class get_set<2, T> {

    public:
        typedef typename T::get_z_type type;

        static type get(const T &a) { return a.get_z(); }

        template<typename V>
        static void set(T &a, const V &v) { a.set_z(v); }
    };

    template<typename T>
    class get_set<3, T> {

    public:
        typedef typename T::get_w_type type;

        static type get(const T &a) { return a.get_w(); }

        template<typename V>
        static void set(T &a, const V &v) { a.set_w(v); }
    };

    enum e_float4_fields {
        float4_fields_none = 0,
        float4_fields_x,
        float4_fields_y,
        float4_fields_xy,
        float4_fields_z,
        float4_fields_xz,
        float4_fields_yz,
        float4_fields_xyz,
        float4_fields_w,
        float4_fields_xw,
        float4_fields_yw,
        float4_fields_xyw,
        float4_fields_zw,
        float4_fields_xzw,
        float4_fields_yzw,
        float4_fields_xyzw
    };

    inline void math_init() {}

    class float4;

    typedef f32 float1;
    typedef f32 float_broadcast;

    template<typename T>
    T select(bool cond, const T &t, const T &f) {
        return cond ? t : f;
    }

    inline f32 multiply_identity(f32) { return 1.0f; }

    inline f32 min(f32 a, f32 b) { return a < b ? a : b; }
    inline f32 max(f32 a, f32 b) { return a > b ? a : b; }
    inline f32 abs(f32 a)        { return fabsf(a); }

    inline f32 approx_inv(f32 a) { return 1.0f / a; }
    inline f32 inv(f32 a)        { return 1.0f / a; }
    inline f32 inv_safe(f32 a)   { return a ? inv(a) : 0.0f; }

    inline f32 sqrt(f32 a)         { return sqrtf(a); }
    inline f32 approx_rsqrt(f32 a) { return 1.0f / sqrtf(a); }
    inline f32 rsqrt(f32 a)        { return 1.0f / sqrtf(a); }

    template<typename T>
    T sqrt_abs(const T &a) {
        return sqrt(abs(a));
    }

    template<typename T>
    T rsqrt_abs(const T &a) {
        return rsqrt(abs(a));
    }

    inline f32 rsqrt_safe(f32 a) { return a ? rsqrt_abs(a) : 0.0f; }

    template<typename T> class float4_packed1;
    template<typename T> class float4_packed2;
    template<typename T> class float4_packed3;
    template<typename T> class float4_packed4;

    template<typename T>
    struct float1_constant {
        f32 x;
        f32 pad_y, pad_z, pad_w;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T>
    struct float2_constant {
        f32 x, y;
        f32 pad_z, pad_w;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T>
    struct float3_constant {
        f32 x, y, z;
        f32 pad_w;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T, i32 n = 1> struct float4_constant;

    template<typename T>
    struct float4_constant<T, 1> {
        f32 x, y, z, w;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T>
    struct float4_constant<T, 2> {
        struct { f32 x, y, z, w; } row1;
        struct { f32 x, y, z, w; } row2;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T>
    struct float4_constant<T, 3> {
        struct { f32 x, y, z, w; } row1;
        struct { f32 x, y, z, w; } row2;
        struct { f32 x, y, z, w; } row3;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    template<typename T>
    struct float4_constant<T, 4> {
        struct { f32 x, y, z, w; } row1;
        struct { f32 x, y, z, w; } row2;
        struct { f32 x, y, z, w; } row3;
        struct { f32 x, y, z, w; } row4;

        operator const T&() const { return reinterpret_cast<const T&>(*this); }
    };

    inline const float4_constant<float4> float4_ones   = { 1.0f, 1.0f, 1.0f, 1.0f };
    inline const float4_constant<float4> float4_twos   = { 2.0f, 2.0f, 2.0f, 2.0f };
    inline const float4_constant<float4> float4_threes = { 3.0f, 3.0f, 3.0f, 3.0f };
    inline const float4_constant<float4> float4_half   = { 0.5f, 0.5f, 0.5f, 0.5f };
    inline const float4_constant<float4> float4_zeroes = { 0.0f, 0.0f, 0.0f, 0.0f };

    inline const f32                     float_kfloor  = 12582912.0f;
    inline const float1_constant<float1> float1_kfloor = { float_kfloor };
    inline const float4_constant<float4> float4_kfloor = { float_kfloor, float_kfloor, float_kfloor, float_kfloor };

    inline const float1_constant<float1> float1_zero    = { 0.0f };
    inline const float1_constant<float1> float1_one     = { 1.0f };
    inline const float1_constant<float1> float1_neg_one = { -1.0f };

    class float4 {

        template<i32 dst, i32 src> friend class float4_axis;
        friend class float4_acc;

    protected:
        f32 x, y, z, w;

    public:
        typedef float4_packed1<float4> packed1;
        typedef float4_packed2<float4> packed2;
        typedef float4_packed3<float4> packed3;
        typedef float4_packed4<float4> packed4;
        typedef float4_packed4<float4> packed;

        typedef const float4_constant<float4> constant;

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        float4() {}
        float4(f32 v) : x(v),
                        y(v),
                        z(v),
                        w(v) {}
        float4(f32 x, f32 y, f32 z) : x(x),
                                      y(y),
                                      z(z) {}
        float4(f32 x, f32 y, f32 z, f32 w) : x(x),
                                             y(y),
                                             z(z),
                                             w(w) {}

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
        template<typename T>       f32 &operator[](T i)       { return ((f32*)this)[i]; }

        template<i32 dx, i32 dy, i32 dz, i32 dw>
        friend float4 swizzle(const float4 &a) {
            enum { code = dx | (dy << 2) | (dz << 4) | (dw << 6) };

            return float4((code & 0x03) == 0x00 ? a.x : (code & 0x03) == 0x01 ? a.y : (code & 0x03) == 0x02 ? a.z : a.w,
                          (code & 0x0C) == 0x00 ? a.x : (code & 0x0C) == 0x04 ? a.y : (code & 0x0C) == 0x08 ? a.z : a.w,
                          (code & 0x30) == 0x00 ? a.x : (code & 0x30) == 0x10 ? a.y : (code & 0x30) == 0x20 ? a.z : a.w,
                          (code & 0xC0) == 0x00 ? a.x : (code & 0xC0) == 0x40 ? a.y : (code & 0xC0) == 0x80 ? a.z : a.w);
        }

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
        const get_w_type &get_w() const { return w; }

        template<typename V> void set_x(const V &val) { ((f32*)this)[0] = (f32)val; }
        template<typename V> void set_y(const V &val) { ((f32*)this)[1] = (f32)val; }
        template<typename V> void set_z(const V &val) { ((f32*)this)[2] = (f32)val; }
        template<typename V> void set_w(const V &val) { ((f32*)this)[3] = (f32)val; }

        template<i32 n, typename V>
        friend void set(float4 &a, const V &val) { a[n] = (f32)val; }

        template<i32 f> const f32 &get() const { return ((const f32*)this)[f]; }

        template<i32 f>
        friend const f32 &get(const float4 &a) { return ((const f32*)&a)[f]; }

        void load1(const f32* p) { x = p[0]; }
        void load2(const f32* p) { x = p[0]; y = p[1]; }
        void load3(const f32* p) { x = p[0]; y = p[1]; z = p[2]; }
        void load4(const f32* p) { x = p[0]; y = p[1]; z = p[2]; w = p[3]; }

        void store1(f32* p) const { p[0] = x; }
        void store2(f32* p) const { p[0] = x; p[1] = y; }
        void store3(f32* p) const { p[0] = x; p[1] = y; p[2] = z; }
        void store4(f32* p) const { p[0] = x; p[1] = y; p[2] = z; p[3] = w; }

        friend float4 operator-(const float4 &a) {
            return float4(-a.x, -a.y, -a.z, -a.w);
        }

        friend float4 abs(const float4 &a) {
            return float4(fabsf(a.x), fabsf(a.y), fabsf(a.z), fabsf(a.w));
        }

        friend float4 approx_inv(const float4 &a) {
            return float4(1.0f / a.x, 1.0f / a.y, 1.0f / a.z, 1.0f / a.w);
        }

        friend float4 operator~(const float4 &a) {
            return float4(1.0f / a.x, 1.0f / a.y, 1.0f / a.z, 1.0f / a.w);
        }

        friend float4 multiply_identity(const float4&) {
            float4 ones = float4_ones;

            return ones;
        }

        friend float4 inv(const float4 &a)  { return ~a; }
        friend float4 inv3(const float4 &a) { return inv(a); }

        friend float4 sqrt(const float4 &a) {
            return float4(sqrtf(a.x), sqrtf(a.y), sqrtf(a.z), sqrtf(a.w));
        }

        friend float4 sqrt3(const float4 &a) { return sqrt(a); }

        friend float4 approx_rsqrt(const float4 &a) {
            return float4(1.0f / sqrtf(a.x), 1.0f / sqrtf(a.y), 1.0f / sqrtf(a.z), 1.0f / sqrtf(a.w));
        }

        friend float4 rsqrt(const float4 &a) {
            return float4(1.0f / sqrtf(a.x), 1.0f / sqrtf(a.y), 1.0f / sqrtf(a.z), 1.0f / sqrtf(a.w));
        }

        friend float4 rsqrt3(const float4 &a) { return rsqrt(a); }

        friend float4 operator+(const float4 &a, const float4 &b) {
            return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }

        friend float4 operator+(const float4 &a, const float1 &b) {
            return float4(a.x + b, a.y + b, a.z + b, a.w + b);
        }

        friend float4 operator+(const float1 &a, const float4 &b) { return b + a; }

        friend float4 operator-(const float4 &a, const float4 &b) {
            return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
        }

        friend float4 operator-(const float4 &a, const float1 &b) {
            return float4(a.x - b, a.y - b, a.z - b, a.w - b);
        }

        friend float4 operator-(const float1 &a, const float4 &b) {
            return float4(a - b.x, a - b.y, a - b.z, a - b.w);
        }

        friend float4 operator*(const float4 &a, const float4 &b) {
            return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
        }

        friend float4 operator*(const float4 &a, const float1 &b) {
            return float4(a.x * b, a.y * b, a.z * b, a.w * b);
        }

        friend float4 operator*(const float1 &a, const float4 &b) { return b * a; }

        friend float4 operator/(const float4 &a, const float4 &b) {
            return float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
        }

        friend float4 operator/(const float4 &a, const float1 &b) {
            f32 invb = 1.0f / b;

            return float4(a.x * invb, a.y * invb, a.z * invb, a.w * invb);
        }

        friend float4 operator/(const float1 &a, const float4 &b) { return float4(a) / b; }

        friend float4 min(const float4 &a, const float4 &b) {
            return float4(a.x < b.x ? a.x : b.x,
                          a.y < b.y ? a.y : b.y,
                          a.z < b.z ? a.z : b.z,
                          a.w < b.w ? a.w : b.w);
        }

        friend float4 min(const float4 &a, const float1 &b) {
            return float4(a.x < b ? a.x : b,
                          a.y < b ? a.y : b,
                          a.z < b ? a.z : b,
                          a.w < b ? a.w : b);
        }

        friend float4 min(const float1 &a, const float4 &b) { return min(b, a); }

        friend float4 max(const float4 &a, const float4 &b) {
            return float4(a.x > b.x ? a.x : b.x,
                          a.y > b.y ? a.y : b.y,
                          a.z > b.z ? a.z : b.z,
                          a.w > b.w ? a.w : b.w);
        }

        friend float4 max(const float4 &a, const float1 &b) {
            return float4(a.x > b ? a.x : b,
                          a.y > b ? a.y : b,
                          a.z > b ? a.z : b,
                          a.w > b ? a.w : b);
        }

        friend float4 max(const float1 &a, const float4 &b) { return max(b, a); }

        friend float4 select_eq(const float4 &a, const float4 &eq, const float4 &ne) {
            return float4(a.x == 0.0f ? eq.x : ne.x,
                          a.y == 0.0f ? eq.y : ne.y,
                          a.z == 0.0f ? eq.z : ne.z,
                          a.w == 0.0f ? eq.w : ne.w);
        }

        friend float4 select_eq(const float4 &a, const float4 &b, const float4 &eq, const float4 &ne) {
            return float4(a.x == b.x ? eq.x : ne.x,
                          a.y == b.y ? eq.y : ne.y,
                          a.z == b.z ? eq.z : ne.z,
                          a.w == b.w ? eq.w : ne.w);
        }

        friend float4 select_ne(const float4 &a, const float4 &ne, const float4 &eq)                   { return select_eq(a, eq, ne); }
        friend float4 select_ne(const float4 &a, const float4 &b,  const float4 &ne, const float4 &eq) { return select_eq(a, b, eq, ne); }

        friend float4 select_ge(const float4 &a, const float4 &ge, const float4 &lt) {
            return float4(a.x >= 0.0f ? ge.x : lt.x,
                          a.y >= 0.0f ? ge.y : lt.y,
                          a.z >= 0.0f ? ge.z : lt.z,
                          a.w >= 0.0f ? ge.w : lt.w);
        }

        friend float4 select_ge(const float4 &a, const float4 &b, const float4 &ge, const float4 &lt) {
            return float4(a.x >= b.x ? ge.x : lt.x,
                          a.y >= b.y ? ge.y : lt.y,
                          a.z >= b.z ? ge.z : lt.z,
                          a.w >= b.w ? ge.w : lt.w);
        }

        friend float4 select_lt(const float4 &a, const float4 &lt, const float4 &ge)                   { return select_ge(a, ge, lt); }
        friend float4 select_lt(const float4 &a, const float4 &b,  const float4 &lt, const float4 &ge) { return select_ge(a, b, ge, lt); }
        friend float4 select_le(const float4 &a, const float4 &le, const float4 &gt)                   { return select_ge(-a, le, gt); }
        friend float4 select_le(const float4 &a, const float4 &b,  const float4 &le, const float4 &gt) { return select_ge(b, a, le, gt); }
        friend float4 select_gt(const float4 &a, const float4 &gt, const float4 &le)                   { return select_ge(-a, le, gt); }
        friend float4 select_gt(const float4 &a, const float4 &b,  const float4 &gt, const float4 &le) { return select_ge(b, a, le, gt); }

        friend float4 mask_eq(const float4 &a, const float4 &eq) {
            return float4(a.x == 0.0f ? eq.x : 0.0f,
                          a.y == 0.0f ? eq.y : 0.0f,
                          a.z == 0.0f ? eq.z : 0.0f,
                          a.w == 0.0f ? eq.w : 0.0f);
        }

        friend float4 mask_eq(const float4 &a, const float4 &b, const float4 &eq) {
            return float4(a.x == b.x ? eq.x : 0.0f,
                          a.y == b.y ? eq.y : 0.0f,
                          a.z == b.z ? eq.z : 0.0f,
                          a.w == b.w ? eq.w : 0.0f);
        }

        friend float4 mask_ne(const float4 &a, const float4 &ne) {
            return float4(a.x != 0.0f ? ne.x : 0.0f,
                          a.y != 0.0f ? ne.y : 0.0f,
                          a.z != 0.0f ? ne.z : 0.0f,
                          a.w != 0.0f ? ne.w : 0.0f);
        }

        friend float4 mask_ne(const float4 &a, const float4 &b, const float4 &ne) {
            return float4(a.x != b.x ? ne.x : 0.0f,
                          a.y != b.y ? ne.y : 0.0f,
                          a.z != b.z ? ne.z : 0.0f,
                          a.w != b.w ? ne.w : 0.0f);
        }

        friend float4 mask_ge(const float4 &a, const float4 &ge) {
            return float4(a.x >= 0.0f ? ge.x : 0.0f,
                          a.y >= 0.0f ? ge.y : 0.0f,
                          a.z >= 0.0f ? ge.z : 0.0f,
                          a.w >= 0.0f ? ge.w : 0.0f);
        }

        friend float4 mask_ge(const float4 &a, const float4 &b, const float4 &ge) {
            return float4(a.x >= b.x ? ge.x : 0.0f,
                          a.y >= b.y ? ge.y : 0.0f,
                          a.z >= b.z ? ge.z : 0.0f,
                          a.w >= b.w ? ge.w : 0.0f);
        }

        friend float4 mask_lt(const float4 &a, const float4 &lt) {
            return float4(a.x < 0.0f ? lt.x : 0.0f,
                          a.y < 0.0f ? lt.y : 0.0f,
                          a.z < 0.0f ? lt.z : 0.0f,
                          a.w < 0.0f ? lt.w : 0.0f);
        }

        friend float4 mask_lt(const float4 &a, const float4 &b, const float4 &lt) {
            return float4(a.x < b.x ? lt.x : 0.0f,
                          a.y < b.y ? lt.y : 0.0f,
                          a.z < b.z ? lt.z : 0.0f,
                          a.w < b.w ? lt.w : 0.0f);
        }

        friend float4 mask_le(const float4 &a, const float4 &le)                  { return mask_ge(-a, le); }
        friend float4 mask_le(const float4 &a, const float4 &b, const float4 &le) { return mask_ge(b, a, le); }
        friend float4 mask_gt(const float4 &a, const float4 &gt)                  { return mask_lt(-a, gt); }
        friend float4 mask_gt(const float4 &a, const float4 &b, const float4 &gt) { return mask_lt(b, a, gt); }

        friend float4 apply_sign(const float4 &signs, const float4 &val) {
            return float4(signs.x < 0.0f ? -val.x : val.x,
                          signs.y < 0.0f ? -val.y : val.y,
                          signs.z < 0.0f ? -val.z : val.z,
                          signs.w < 0.0f ? -val.w : val.w);
        }

        friend bool operator==(const float4 &a, const float4 &b) {
            return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
        }

        friend bool operator!=(const float4 &a, const float4 &b) { return !(a == b); }

        template<i32 mask>
        friend bool compare_all_eq(const float4 &a, const float4 &b) {
            return ((mask & 1) == 0 || a.x == b.x) &&
                   ((mask & 2) == 0 || a.y == b.y) &&
                   ((mask & 4) == 0 || a.z == b.z) &&
                   ((mask & 8) == 0 || a.w == b.w);
        }

        template<i32 mask>
        friend bool compare_any_ne(const float4 &a, const float4 &b) { return !compare_all_eq<mask>(a, b); }

        template<i32 mask>
        friend bool compare_any_eq(const float4 &a, const float4 &b) {
            return ((mask & 1) && a.x == b.x) ||
                   ((mask & 2) && a.y == b.y) ||
                   ((mask & 4) && a.z == b.z) ||
                   ((mask & 8) && a.w == b.w);
        }

        template<i32 mask>
        friend bool compare_all_ne(const float4 &a, const float4 &b) { return !compare_any_eq<mask>(a, b); }

        template<i32 mask>
        friend bool compare_all_lt(const float4 &a, const float4 &b) {
            return ((mask & 1) == 0 || a.x < b.x) &&
                   ((mask & 2) == 0 || a.y < b.y) &&
                   ((mask & 4) == 0 || a.z < b.z) &&
                   ((mask & 8) == 0 || a.w < b.w);
        }

        template<i32 mask> friend bool compare_any_ge(const float4 &a, const float4 &b) { return !compare_all_lt<mask>(a, b); }
        template<i32 mask> friend bool compare_all_gt(const float4 &a, const float4 &b) { return  compare_all_lt<mask>(b, a); }
        template<i32 mask> friend bool compare_any_le(const float4 &a, const float4 &b) { return !compare_all_lt<mask>(b, a); }

        template<i32 mask>
        friend bool compare_any_lt(const float4 &a, const float4 &b) {
            return ((mask & 1) && a.x < b.x) ||
                   ((mask & 2) && a.y < b.y) ||
                   ((mask & 4) && a.z < b.z) ||
                   ((mask & 8) && a.w < b.w);
        }

        template<i32 mask> friend bool compare_all_ge(const float4 &a, const float4 &b) { return !compare_any_lt<mask>(a, b); }
        template<i32 mask> friend bool compare_any_gt(const float4 &a, const float4 &b) { return  compare_any_lt<mask>(b, a); }
        template<i32 mask> friend bool compare_all_le(const float4 &a, const float4 &b) { return !compare_any_lt<mask>(b, a); }

        friend float4 inv_safe(const float4 &a)   { return mask_ne(a, inv(a)); }
        friend float4 rsqrt_safe(const float4 &a) { return mask_ne(a, rsqrt_abs(a)); }

        template<typename T> float4 &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> float4 &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> float4 &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> float4 &operator/=(const T &a) { return *this = *this / a; }

        friend float1 sum2(const float4 &a) { return a.x + a.y; }
        friend float1 sum3(const float4 &a) { return a.x + a.y + a.z; }
        friend float1 sum4(const float4 &a) { return a.x + a.y + a.z + a.w; }

        friend float1 dot3(const float4 &a, const float4 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        friend float1 dot4(const float4 &a, const float4 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        friend float1 dot34(const float4 &a, const float4 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z + b.w;
        }

        friend float4 partial_cross3(const float4 &a, const float4 &b) {
            return float4(a.y * b.z,
                          a.z * b.x,
                          a.x * b.y);
        }

        friend float4 fast_cross3(const float4 &a, const float4 &b) {
            return float4(a.y * b.z - a.z * b.y,
                          a.z * b.x - a.x * b.z,
                          a.x * b.y - a.y * b.x,
                          0.0f);
        }

        friend float4 cross3(const float4 &a, const float4 &b) {
            return float4(a.y * b.z - a.z * b.y,
                          a.z * b.x - a.x * b.z,
                          a.x * b.y - a.y * b.x,
                          0.0f);
        }

        friend float4 mul_quat(const float4 &a, const float4 &b) {
            return float4((f32)( (f64)a.x * b.w + (f64)a.y * b.z - (f64)a.z * b.y + (f64)a.w * b.x),
                          (f32)(-(f64)a.x * b.z + (f64)a.y * b.w + (f64)a.z * b.x + (f64)a.w * b.y),
                          (f32)( (f64)a.x * b.y - (f64)a.y * b.x + (f64)a.z * b.w + (f64)a.w * b.z),
                          (f32)(-(f64)a.x * b.x - (f64)a.y * b.y - (f64)a.z * b.z + (f64)a.w * b.w));
        }

        friend void   quat_to_matrix(const float4 &q, float4 &r0, float4 &r1, float4 &r2);
        friend float4 quat_rotate(const float4 &q, const float4 &vec);

        friend void transpose(      float4 &dst1,       float4 &dst2,       float4 &dst3,
                              const float4 &src1, const float4 &src2, const float4 &src3) {

            dst1.x = src1.x; dst1.y = src2.x; dst1.z = src3.x;
            dst2.x = src1.y; dst2.y = src2.y; dst2.z = src3.y;
            dst3.x = src1.z; dst3.y = src2.z; dst3.z = src3.z;
        }

        friend void transpose(float4 &dst1, float4 &dst2, float4 &dst3) {
            f32 tmp;

            tmp = dst2.x; dst2.x = dst1.y; dst1.y = tmp;
            tmp = dst3.x; dst3.x = dst1.z; dst1.z = tmp;
            tmp = dst3.y; dst3.y = dst2.z; dst2.z = tmp;
        }

        friend void transpose(      float4 &dst1,       float4 &dst2,       float4 &dst3, float4 &dst4,
                              const float4 &src1, const float4 &src2, const float4 &src3) {

            dst1.x = src1.x; dst1.y = src2.x; dst1.z = src3.x;
            dst2.x = src1.y; dst2.y = src2.y; dst2.z = src3.y;
            dst3.x = src1.z; dst3.y = src2.z; dst3.z = src3.z;
            dst4.x = src1.w; dst4.y = src2.w; dst4.z = src3.w;
        }

        friend void transpose(      float4 &dst1,       float4 &dst2,       float4 &dst3,
                              const float4 &src1, const float4 &src2, const float4 &src3, const float4 &src4) {

            dst1.x = src1.x; dst1.y = src2.x; dst1.z = src3.x; dst1.w = src4.x;
            dst2.x = src1.y; dst2.y = src2.y; dst2.z = src3.y; dst2.w = src4.y;
            dst3.x = src1.z; dst3.y = src2.z; dst3.z = src3.z; dst3.w = src4.z;
        }

        friend void transpose(      float4 &dst1,       float4 &dst2,       float4 &dst3,       float4 &dst4,
                              const float4 &src1, const float4 &src2, const float4 &src3, const float4 &src4) {

            dst1.x = src1.x; dst1.y = src2.x; dst1.z = src3.x; dst1.w = src4.x;
            dst2.x = src1.y; dst2.y = src2.y; dst2.z = src3.y; dst2.w = src4.y;
            dst3.x = src1.z; dst3.y = src2.z; dst3.z = src3.z; dst3.w = src4.z;
            dst4.x = src1.w; dst4.y = src2.w; dst4.z = src3.w; dst4.w = src4.w;
        }

        friend void transpose(float4 &dst1, float4 &dst2, float4 &dst3, float4 &dst4) {
            f32 tmp;

            tmp = dst2.x; dst2.x = dst1.y; dst1.y = tmp;
            tmp = dst3.x; dst3.x = dst1.z; dst1.z = tmp;
            tmp = dst4.x; dst4.x = dst1.w; dst1.w = tmp;
            tmp = dst3.y; dst3.y = dst2.z; dst2.z = tmp;
            tmp = dst4.y; dst4.y = dst2.w; dst2.w = tmp;
            tmp = dst4.z; dst4.z = dst3.w; dst3.w = tmp;
        }

        friend void inv_mat(      float4 &dst1,       float4 &dst2,       float4 &dst3,
                            const float4 &src1, const float4 &src2, const float4 &src3) {

            float4 c0, c1, c2;
            transpose(c0, c1, c2, src2, src3, src1);

            float4 c0s = swizzle<1, 2, 0, 3>(c0);
            float4 c1s = swizzle<1, 2, 0, 3>(c1);
            float4 c2s = swizzle<1, 2, 0, 3>(c2);

            dst1 = c1 * c2s - c1s * c2;
            dst2 = c2 * c0s - c2s * c0;
            dst3 = c0 * c1s - c0s * c1;

            float_broadcast invdet = inv(dot3(swizzle<2, 0, 1, 3>(c0), dst1));

            dst1 *= invdet;
            dst2 *= invdet;
            dst3 *= invdet;
        }

        // inlined @ sub_9D2B70
        friend void inv_mat(      float4 &dst1,       float4 &dst2,       float4 &dst3,       float4 &dst4,
                            const float4 &src1, const float4 &src2, const float4 &src3, const float4 &src4) {

            const float4 &x = src1;
            const float4 &y = src2;
            const float4 &z = src3;
            const float4 &w = src4;

            // 2x2 minors of the bottom two rows; the ones with the w column round each product first
            f32 lower_yz = (f32)((f64)z.y * w.z - (f64)z.z * w.y);
            f32 lower_zx = (f32)((f64)z.z * w.x - (f64)z.x * w.z);
            f32 lower_xy = (f32)((f64)z.x * w.y - (f64)z.y * w.x);
            f32 lower_zw = z.z * w.w - z.w * w.z;
            f32 lower_yw = z.y * w.w - z.w * w.y;
            f32 lower_xw = z.x * w.w - z.w * w.x;

            // and of the top two rows
            f32 upper_yz = (f32)((f64)x.y * y.z - (f64)x.z * y.y);
            f32 upper_zx = (f32)((f64)x.z * y.x - (f64)x.x * y.z);
            f32 upper_xy = (f32)((f64)x.x * y.y - (f64)x.y * y.x);
            f32 upper_zw = x.z * y.w - x.w * y.z;
            f32 upper_yw = x.y * y.w - x.w * y.y;
            f32 upper_xw = x.x * y.w - x.w * y.x;

            f32 cofactor_xx = (y.w * lower_yz + (f32)((f64)y.y * lower_zw - (f64)y.z * lower_yw)) + 0.0f;
            f32 cofactor_xy = (y.w * lower_zx + (f32)((f64)y.z * lower_xw - (f64)y.x * lower_zw)) + 0.0f;
            f32 cofactor_xz = (y.w * lower_xy + (f32)((f64)y.x * lower_yw - (f64)y.y * lower_xw)) + 0.0f;

            f32 determinant = (f32)((f64)cofactor_xz * x.z + ((f64)cofactor_xy * x.y + (f64)cofactor_xx * x.x));

            f32 y_w_lane = 0.0f * y.w + 0.0f;
            f32 x_w_lane = 0.0f - 0.0f * x.w;
            f32 w_w_lane = 0.0f * w.w + 0.0f;
            f32 z_w_lane = 0.0f - 0.0f * z.w;

            dst1 = float4(cofactor_xx,
                          cofactor_xy,
                          cofactor_xz,
                          y_w_lane - (f32)((f64)lower_xy * y.z + ((f64)lower_zx * y.y + (f64)lower_yz * y.x)));

            dst2 = float4(((f32)((f64)lower_yw * x.z - (f64)lower_zw * x.y) - x.w * lower_yz) + 0.0f,
                          ((f32)((f64)lower_zw * x.x - (f64)lower_xw * x.z) - x.w * lower_zx) + 0.0f,
                          ((f32)((f64)lower_xw * x.y - (f64)lower_yw * x.x) - x.w * lower_xy) + 0.0f,
                          (f32)((f64)lower_xy * x.z + ((f64)lower_zx * x.y + (f64)lower_yz * x.x)) + x_w_lane);

            dst3 = float4((w.w * upper_yz + (f32)((f64)upper_zw * w.y - (f64)upper_yw * w.z)) + 0.0f,
                          (w.w * upper_zx + (f32)((f64)upper_xw * w.z - (f64)upper_zw * w.x)) + 0.0f,
                          (w.w * upper_xy + (f32)((f64)upper_yw * w.x - (f64)upper_xw * w.y)) + 0.0f,
                          w_w_lane - (f32)((f64)upper_xy * w.z + ((f64)upper_yz * w.x + (f64)upper_zx * w.y)));

            dst4 = float4(((f32)((f64)upper_yw * z.z - (f64)upper_zw * z.y) - z.w * upper_yz) + 0.0f,
                          ((f32)((f64)upper_zw * z.x - (f64)upper_xw * z.z) - z.w * upper_zx) + 0.0f,
                          ((f32)((f64)upper_xw * z.y - (f64)upper_yw * z.x) - z.w * upper_xy) + 0.0f,
                          (f32)((f64)upper_xy * z.z + ((f64)upper_yz * z.x + (f64)upper_zx * z.y)) + z_w_lane);

            // a singular matrix keeps the unscaled cofactors
            if (determinant != 0.0f) {
                float_broadcast invdet = 1.0f / determinant;

                dst1 *= invdet;
                dst2 *= invdet;
                dst3 *= invdet;
                dst4 *= invdet;
            }
        }

        friend float4 line_intersects_box(const float4 &p, const float4 &d, const float4 &lens) {
            float4 abs_d = abs(d);

            return (lens * swizzle<1, 2, 0, 3>(abs_d) + swizzle<1, 2, 0, 3>(lens) * abs_d) - abs(p * swizzle<1, 2, 0, 3>(d) - swizzle<1, 2, 0, 3>(p) * d);
        }
    };

    ASSERT_SIZEOF(float4, 0x10);

    template<typename T>
    class float4_packed1 {

    protected:
        f32 x;

    public:
        typedef f32 get_x_type;

        float4_packed1() {}
        float4_packed1(f32 x) : x(x) {}
        float4_packed1(const T &a) { *this = a; }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        get_x_type get_x() const { return x; }

        template<typename V> void set_x(const V &val) { x = val; }

        operator const T() const {
            float4 result;
            result.load1((f32*)this);

            return reinterpret_cast<const T&>(result);
        }

        float4_packed1 &operator=(const T &a) {
            reinterpret_cast<const float4&>(a).store1((f32*)this);

            return *this;
        }

        template<typename V> float4_packed1 &operator+=(const V &a) { return *this = *this + a; }
        template<typename V> float4_packed1 &operator-=(const V &a) { return *this = *this - a; }
        template<typename V> float4_packed1 &operator*=(const V &a) { return *this = *this * a; }
        template<typename V> float4_packed1 &operator/=(const V &a) { return *this = *this / a; }
    };

    template<typename T> T select(bool cond, const T &t, const float4_packed1<T> &f) { return cond ? t : (const T)f; }
    template<typename T> T select(bool cond, const float4_packed1<T> &t, const T &f) { return cond ? (const T)t : f; }

    template<typename T>
    class float4_packed2 {

    protected:
        f32 x, y;

    public:
        typedef f32 get_x_type;
        typedef f32 get_y_type;

        float4_packed2() {}
        float4_packed2(f32 x, f32 y) : x(x), y(y) {}
        float4_packed2(const T &a) { *this = a; }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        get_x_type get_x() const { return x; }
        get_y_type get_y() const { return y; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }

        operator const T() const {
            float4 result;
            result.load2((f32*)this);

            return reinterpret_cast<const T&>(result);
        }

        float4_packed2 &operator=(const T &a) {
            reinterpret_cast<const float4&>(a).store2((f32*)this);

            return *this;
        }

        template<typename V> float4_packed2 &operator+=(const V &a) { return *this = *this + a; }
        template<typename V> float4_packed2 &operator-=(const V &a) { return *this = *this - a; }
        template<typename V> float4_packed2 &operator*=(const V &a) { return *this = *this * a; }
        template<typename V> float4_packed2 &operator/=(const V &a) { return *this = *this / a; }
    };

    template<typename T> T select(bool cond, const T &t, const float4_packed2<T> &f) { return cond ? t : (const T)f; }
    template<typename T> T select(bool cond, const float4_packed2<T> &t, const T &f) { return cond ? (const T)t : f; }

    template<typename T>
    class float4_packed3 {

    protected:
        f32 x, y, z;

    public:
        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;

        float4_packed3() {}
        float4_packed3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
        float4_packed3(const T &a) { *this = a; }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        get_x_type get_x() const { return x; }
        get_y_type get_y() const { return y; }
        get_z_type get_z() const { return z; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }

        operator const T() const {
            float4 result;
            result.load3((f32*)this);

            return reinterpret_cast<const T&>(result);
        }

        float4_packed3 &operator=(const T &a) {
            reinterpret_cast<const float4&>(a).store3((f32*)this);

            return *this;
        }

        template<typename V> float4_packed3 &operator+=(const V &a) { return *this = *this + a; }
        template<typename V> float4_packed3 &operator-=(const V &a) { return *this = *this - a; }
        template<typename V> float4_packed3 &operator*=(const V &a) { return *this = *this * a; }
        template<typename V> float4_packed3 &operator/=(const V &a) { return *this = *this / a; }
    };

    template<typename T> T select(bool cond, const T &t, const float4_packed3<T> &f) { return cond ? t : (const T)f; }
    template<typename T> T select(bool cond, const float4_packed3<T> &t, const T &f) { return cond ? (const T)t : f; }

    template<typename T>
    class float4_packed4 {

    protected:
        f32 x, y, z, w;

    public:
        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        float4_packed4() {}
        float4_packed4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
        float4_packed4(const T &a) { *this = a; }

        template<typename I> const f32 &operator[](I i) const { return ((const f32*)this)[i]; }
        template<typename I>       f32 &operator[](I i)       { return ((f32*)this)[i]; }

        get_x_type get_x() const { return x; }
        get_y_type get_y() const { return y; }
        get_z_type get_z() const { return z; }
        get_w_type get_w() const { return w; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }
        template<typename V> void set_w(const V &val) { w = val; }

        operator const T&() const { return reinterpret_cast<const T&>(*this); }

        float4_packed4 &operator=(const T &a) {
            reinterpret_cast<T&>(*this) = a;

            return *this;
        }

        template<typename V> float4_packed4 &operator+=(const V &a) { return *this = *this + a; }
        template<typename V> float4_packed4 &operator-=(const V &a) { return *this = *this - a; }
        template<typename V> float4_packed4 &operator*=(const V &a) { return *this = *this * a; }
        template<typename V> float4_packed4 &operator/=(const V &a) { return *this = *this / a; }
    };

    template<typename T> T select(bool cond, const T &t, const float4_packed4<T> &f) { return cond ? t : (const T)f; }
    template<typename T> T select(bool cond, const float4_packed4<T> &t, const T &f) { return cond ? (const T)t : f; }

    inline const float4::constant f4_1000 = { 1.0f, 0.0f, 0.0f, 0.0f };
    inline const float4::constant f4_0100 = { 0.0f, 1.0f, 0.0f, 0.0f };
    inline const float4::constant f4_0010 = { 0.0f, 0.0f, 1.0f, 0.0f };
    inline const float4::constant f4_0001 = { 0.0f, 0.0f, 0.0f, 1.0f };

    template<i32 dst, i32 src = dst>
    class float4_axis {

    protected:
        float4 v;

    public:
        explicit float4_axis(const float4 &a) : v(a) {}

        operator float4() const {
            return float4(dst == 0 ? (f32)get<src>(v) : 0.0f,
                          dst == 1 ? (f32)get<src>(v) : 0.0f,
                          dst == 2 ? (f32)get<src>(v) : 0.0f,
                          dst == 3 ? (f32)get<src>(v) : 0.0f);
        }
    };

    template<i32 dst>
    class float1_axis {

    protected:
        float1 v;

    public:
        explicit float1_axis(const float1 &a) : v(a) {}

        operator float4() const {
            return float4(dst == 0 ? (f32)v : 0.0f,
                          dst == 1 ? (f32)v : 0.0f,
                          dst == 2 ? (f32)v : 0.0f,
                          dst == 3 ? (f32)v : 0.0f);
        }
    };

    class float4_acc {

    protected:
        float4 acc;

    public:
        float4_acc() {}

        template<typename T> void load(const T &b) { acc = b; }

        template<typename T> void load_add(const float4 &a, const T &b) { acc = a + b; }
        template<typename T> void load_sub(const float4 &a, const T &b) { acc = a - b; }
        template<typename T> void load_mul(const float4 &a, const T &b) { acc = a * b; }

        template<typename T> void add(const T &b) { acc += b; }
        template<typename T> void sub(const T &b) { acc -= b; }

        void addmul(const float4 &a, const float4 &b) {
            acc = float4((f32)((f64)acc.x + (f64)a.x * b.x),
                         (f32)((f64)acc.y + (f64)a.y * b.y),
                         (f32)((f64)acc.z + (f64)a.z * b.z),
                         (f32)((f64)acc.w + (f64)a.w * b.w));
        }

        void addmul(const float4 &a, const float1 &b) {
            acc = float4((f32)((f64)acc.x + (f64)a.x * b),
                         (f32)((f64)acc.y + (f64)a.y * b),
                         (f32)((f64)acc.z + (f64)a.z * b),
                         (f32)((f64)acc.w + (f64)a.w * b));
        }

        void submul(const float4 &a, const float4 &b) {
            acc = float4((f32)((f64)acc.x - (f64)a.x * b.x),
                         (f32)((f64)acc.y - (f64)a.y * b.y),
                         (f32)((f64)acc.z - (f64)a.z * b.z),
                         (f32)((f64)acc.w - (f64)a.w * b.w));
        }

        void submul(const float4 &a, const float1 &b) {
            acc = float4((f32)((f64)acc.x - (f64)a.x * b),
                         (f32)((f64)acc.y - (f64)a.y * b),
                         (f32)((f64)acc.z - (f64)a.z * b),
                         (f32)((f64)acc.w - (f64)a.w * b));
        }

        float4 ret() { return acc; }

        template<typename T> float4 ret_add(const T &b) { return acc + b; }
        template<typename T> float4 ret_sub(const T &b) { return acc - b; }

        template<typename T>
        float4 ret_addmul(const float4 &a, const T &b) {
            float4_acc result = *this;
            result.addmul(a, b);

            return result.acc;
        }

        template<typename T>
        float4 ret_submul(const float4 &a, const T &b) {
            float4_acc result = *this;
            result.submul(a, b);

            return result.acc;
        }

        template<typename T>
        void load_floor_pos(const T &b) {
            load(float4(floorf(b.get_x()), floorf(b.get_y()), floorf(b.get_z()), floorf(b.get_w())));
        }

        void floor_pos() {
            acc = float4(floorf(acc.get_x()), floorf(acc.get_y()), floorf(acc.get_z()), floorf(acc.get_w()));
        }

        float4 ret_floor_pos() {
            return float4(floorf(acc.get_x()), floorf(acc.get_y()), floorf(acc.get_z()), floorf(acc.get_w()));
        }

        template<typename T>
        void load_ceil_neg(const T &b) {
            load(float4(ceilf(b.get_x()), ceilf(b.get_y()), ceilf(b.get_z()), ceilf(b.get_w())));
        }

        void ceil_neg() {
            acc = float4(ceilf(acc.get_x()), ceilf(acc.get_y()), ceilf(acc.get_z()), ceilf(acc.get_w()));
        }

        float4 ret_ceil_neg() {
            return float4(ceilf(acc.get_x()), ceilf(acc.get_y()), ceilf(acc.get_z()), ceilf(acc.get_w()));
        }
    };

    inline f64 madd_lane(const float4 &a, i32 n) { return a[n]; }
    inline f64 madd_lane(f32           a, i32)   { return a; }

    template<typename B1>
    float4 madd(const float4 &a1, const B1 &b1) {
        return a1 * b1;
    }

    template<typename B1, typename B2>
    float4 madd(const float4 &a1, const B1 &b1,
                const B2     &b2) {

        auto lane = [&](i32 n) {
            return (f32)(madd_lane(b2, n) + a1[n] * madd_lane(b1, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2) {

        auto lane = [&](i32 n) {
            return (f32)((f64)a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const B3     &b3) {

        auto lane = [&](i32 n) {
            return (f32)(madd_lane(b3, n) + a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const float4 &a3, const B3 &b3) {

        auto lane = [&](i32 n) {
            return (f32)((f64)a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n) + a3[n] * madd_lane(b3, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3, typename B4>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const float4 &a3, const B3 &b3,
                const B4     &b4) {

        auto lane = [&](i32 n) {
            return (f32)(madd_lane(b4, n) + a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n) + a3[n] * madd_lane(b3, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3, typename B4>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const float4 &a3, const B3 &b3,
                const float4 &a4, const B4 &b4) {

        auto lane = [&](i32 n) {
            return (f32)((f64)a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n) + a3[n] * madd_lane(b3, n) + a4[n] * madd_lane(b4, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3, typename B4, typename B5>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const float4 &a3, const B3 &b3,
                const float4 &a4, const B4 &b4,
                const B5     &b5) {

        auto lane = [&](i32 n) {
            return (f32)(madd_lane(b5, n) + a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n) + a3[n] * madd_lane(b3, n) + a4[n] * madd_lane(b4, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    template<typename B1, typename B2, typename B3, typename B4, typename B5>
    float4 madd(const float4 &a1, const B1 &b1,
                const float4 &a2, const B2 &b2,
                const float4 &a3, const B3 &b3,
                const float4 &a4, const B4 &b4,
                const float4 &a5, const B5 &b5) {

        auto lane = [&](i32 n) {
            return (f32)((f64)a1[n] * madd_lane(b1, n) + a2[n] * madd_lane(b2, n) + a3[n] * madd_lane(b3, n) + a4[n] * madd_lane(b4, n) + a5[n] * madd_lane(b5, n));
        };

        return float4(lane(0), lane(1), lane(2), lane(3));
    }

    inline void quat_to_matrix(const float4 &q, float4 &r0, float4 &r1, float4 &r2) {
        const float4 q2   = q + q;
        const float4 wq2  = q2.get_w() * q;
        const float4 nwq2 = -wq2;
        const float1 d    = wq2.get_w() - reinterpret_cast<const float1&>(float1_one);

        r0 = madd(q2, q.get_x(), float4(d,            nwq2.get_z(), wq2.get_y()));
        r1 = madd(q2, q.get_y(), float4(wq2.get_z(),  d,            nwq2.get_x()));
        r2 = madd(q2, q.get_z(), float4(nwq2.get_y(), wq2.get_x(),  d));
    }

    inline float4 quat_rotate(const float4 &q, const float4 &vec) {
        const float4 q2  = q + q;
        const float4 wq2 = q2.get_w() * q;
        const float1 d   = wq2.get_w() - reinterpret_cast<const float1&>(float1_one);
        const float1 qv  = dot3(q, vec);
        const float4 c   = fast_cross3(vec, wq2);

        return madd(q2, qv, vec, d, c);
    }

    inline void axis_sin_cos_to_matrix(const float4 &v, const float1 &s, const float1 &c, float4 &r0, float4 &r1, float4 &r2) {
        const float4 &tv  = v - v * c;
        const float4 &sv  = v * s;
        const float4 &nsv = -sv;

        r0 = madd(tv, v.get_x(), float4(c,           sv.get_z(),  nsv.get_y()));
        r1 = madd(tv, v.get_y(), float4(nsv.get_z(), c,           sv.get_x()));
        r2 = madd(tv, v.get_z(), float4(sv.get_y(),  nsv.get_x(), c));
    }

    inline const float4::constant axis180_negx = { -1.0f,  0.0f,  0.0f, 0.0f };
    inline const float4::constant axis180_negy = {  0.0f, -1.0f,  0.0f, 0.0f };
    inline const float4::constant axis180_negz = {  0.0f,  0.0f, -1.0f, 0.0f };

    inline void axis180_to_matrix(const float4 &v, float4 &r0, float4 &r1, float4 &r2) {
        const float4 &tv = v + v;

        r0 = madd(tv, v.get_x(), reinterpret_cast<const float4&>(axis180_negx));
        r1 = madd(tv, v.get_y(), reinterpret_cast<const float4&>(axis180_negy));
        r2 = madd(tv, v.get_z(), reinterpret_cast<const float4&>(axis180_negz));
    }

    inline f32    trunc(f32 a)           { return (f32)(i32)a; }
    inline float4 trunc(const float4 &a) { return float4(trunc(a[0]), trunc(a[1]), trunc(a[2]), trunc(a[3])); }

    inline f32    floor(f32 a)           { return floorf(a); }
    inline float4 floor(const float4 &a) { return float4(floor(a[0]), floor(a[1]), floor(a[2]), floor(a[3])); }

    inline f32    ceil(f32 a)           { return ceilf(a); }
    inline float4 ceil(const float4 &a) { return float4(ceil(a[0]), ceil(a[1]), ceil(a[2]), ceil(a[3])); }

    template<typename T> T floor_pos(const T &a) { return floor(a); }
    template<typename T> T floor_neg(const T &a) { return floor(a); }
    template<typename T> T ceil_pos(const T &a)  { return ceil(a); }
    template<typename T> T ceil_neg(const T &a)  { return ceil(a); }
    template<typename T> T trunc_pos(const T &a) { return floor_pos(a); }
    template<typename T> T trunc_neg(const T &a) { return ceil_neg(a); }

    template<typename T> T fract(const T &a)     { return a - floor(a); }
    template<typename T> T fract_pos(const T &a) { return a - floor_pos(a); }
    template<typename T> T fract_neg(const T &a) { return a - floor_neg(a); }

    typedef const float1 float1_arg;
    typedef const float4 float4_arg;
}} // treyarch::math
