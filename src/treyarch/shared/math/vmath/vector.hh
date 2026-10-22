#pragma once

#include "treyarch/shared/math/vmath/float4.hh"

namespace treyarch { namespace math {
    template<typename Op, typename T1, typename T2 = void, typename T3 = void> class result_type;

    class op_add;
    class op_sub;
    class op_mul;
    class op_neg;
    class op_inv;
    class op_get_m33;
    class op_get_quaternion;

    template<typename T>
    T power(const T &value, i32 power) {
        T result = multiply_identity(value);

        if (power == 0)
            return result;

        i32 pbits = power >= 0 ? power : -power;

        T base = value;

        for (;;) {
            if (pbits & 1)
                result *= base;

            pbits >>= 1;

            if (!pbits)
                break;

            base *= base;
        }

        if (power < 0)
            return inv(result);
        else
            return result;
    }

    class rep_constant;
    template<bool is_packed = false> class rep_std;
    template<typename T> class rep_type;

    template<i32 n> class vec_axis;
    class vec_axis_zero;
    template<i32 n = 1> class vec_unit;

    template<i32 size = 4, i32 lastval = -1, typename axis = void, typename unit = void, typename rep = rep_std<>> class vec_class;

    typedef vec_class<4, -1>                             vector4;
    typedef vec_class<4, -1, vec_axis_zero>              vector4_zero;
    typedef vec_class<4, -1, void, vec_unit<1>>          unit_vector4;
    typedef vec_class<3,  1>                             position3;
    typedef vec_class<3,  1, vec_axis_zero>              position3_zero;
    typedef vec_class<3,  0>                             dir3;
    typedef vec_class<3,  0, vec_axis_zero>              dir3_zero;
    typedef vec_class<3,  0, vec_axis<0>>                dir_x;
    typedef vec_class<3,  0, vec_axis<1>>                dir_y;
    typedef vec_class<3,  0, vec_axis<2>>                dir_z;
    typedef vec_class<4, -1, vec_axis<3>>                dir_w;
    typedef vec_class<3,  0, void, vec_unit<1>>          unit_dir3;
    typedef vec_class<3,  0, vec_axis<0>, vec_unit<1>>   unit_dir_x;
    typedef vec_class<3,  0, vec_axis<1>, vec_unit<1>>   unit_dir_y;
    typedef vec_class<3,  0, vec_axis<2>, vec_unit<1>>   unit_dir_z;
    typedef vec_class<4, -1, vec_axis<3>, vec_unit<1>>   unit_dir_w;
    typedef vec_class<3,  0, vec_axis<0>, vec_unit<-1>>  unit_neg_dir_x;
    typedef vec_class<3,  0, vec_axis<1>, vec_unit<-1>>  unit_neg_dir_y;
    typedef vec_class<3,  0, vec_axis<2>, vec_unit<-1>>  unit_neg_dir_z;
    typedef vec_class<4, -1, vec_axis<3>, vec_unit<-1>>  unit_neg_dir_w;

    typedef const vector4      vector4_arg;
    typedef const unit_vector4 unit_vector4_arg;
    typedef const position3    position3_arg;
    typedef const dir3         dir3_arg;
    typedef const dir_x        dir_x_arg;
    typedef const dir_y        dir_y_arg;
    typedef const dir_z        dir_z_arg;
    typedef const dir_w        dir_w_arg;
    typedef const unit_dir3    unit_dir3_arg;

    typedef unit_dir_x     unit_dir_x_arg;
    typedef unit_dir_y     unit_dir_y_arg;
    typedef unit_dir_z     unit_dir_z_arg;
    typedef unit_dir_w     unit_dir_w_arg;
    typedef unit_neg_dir_x unit_neg_dir_x_arg;
    typedef unit_neg_dir_y unit_neg_dir_y_arg;
    typedef unit_neg_dir_z unit_neg_dir_z_arg;
    typedef unit_neg_dir_w unit_neg_dir_w_arg;

    template<>
    class vec_class<4, -1, vec_axis_zero> {

    public:
        const float4 &val4()  const { return float4_zeroes; }
        const float4 &val34() const { return float4_zeroes; }

        typedef float1 get_x_type;
        typedef float1 get_y_type;
        typedef float1 get_z_type;
        typedef float1 get_w_type;

        const get_x_type &get_x() const { return float1_zero; }
        const get_y_type &get_y() const { return float1_zero; }
        const get_z_type &get_z() const { return float1_zero; }
        const get_w_type &get_w() const { return float1_zero; }

        vec_class() {}
    };

    template<>
    class vec_class<3, 0, vec_axis_zero> {

    public:
        const float4 &val4()  const { return float4_zeroes; }
        const float4 &val34() const { return float4_zeroes; }

        typedef float1 get_x_type;
        typedef float1 get_y_type;
        typedef float1 get_z_type;
        typedef float1 get_w_type;

        const get_x_type &get_x() const { return float1_zero; }
        const get_y_type &get_y() const { return float1_zero; }
        const get_z_type &get_z() const { return float1_zero; }
        const get_w_type &get_w() const { return float1_zero; }

        vec_class() {}
    };

    template<>
    class vec_class<3, 1, vec_axis_zero> {

    public:
        const float4 &val4()  const { return f4_0001; }
        const float4 &val34() const { return f4_0001; }

        typedef float1 get_x_type;
        typedef float1 get_y_type;
        typedef float1 get_z_type;
        typedef float1 get_w_type;

        const get_x_type &get_x() const { return float1_zero; }
        const get_y_type &get_y() const { return float1_zero; }
        const get_z_type &get_z() const { return float1_zero; }
        const get_w_type &get_w() const { return float1_one; }

        vec_class() {}
    };

    template<>
    class vec_class<4> { // vector4

    protected:
        float4 v;

    public:
        explicit vec_class(const float4 &_v) : v(_v) {}

        const float4 &baseval() const { return v; }
        const float4 &val4()    const { return baseval(); }
        const float4 &val34()   const { return baseval(); }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float4::get_w_type get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return v.get_w(); }

        template<typename V> void set_x(const V &val) { v.set_x(val); }
        template<typename V> void set_y(const V &val) { v.set_y(val); }
        template<typename V> void set_z(const V &val) { v.set_z(val); }
        template<typename V> void set_w(const V &val) { v.set_w(val); }

        vec_class() {}

        template<typename X, typename Y, typename Z, typename W>
        vec_class(const X &x, const Y &y, const Z &z, const W &w) : v(x, y, z, w) {}

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class(const vec_class<S, L, A, U, R> &_v) : v(_v.val4()) {}

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class &operator=(const vec_class<S, L, A, U, R> &_v) { v = _v.val4(); return *this; }

        template<typename T> const f32 &operator[](T i) const { return v[i]; }
        template<typename T>       f32 &operator[](T i)       { return v[i]; }

        typedef       vec_class<4, -1, void, void, rep_std<true>> packed;
        typedef const vec_class<4, -1, void, void, rep_constant>  constant;

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<4, -1, void, vec_unit<1>> { // unit_vector4

    protected:
        float4 v;

    public:
        explicit vec_class(const float4 &_v) : v(_v) {}

        const float4 &baseval() const { return v; }
        const float4 &val4()    const { return baseval(); }
        const float4 &val34()   const { return baseval(); }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float4::get_w_type get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return v.get_w(); }

        vec_class() {}

        template<typename X, typename Y, typename Z, typename W>
        vec_class(const X &x, const Y &y, const Z &z, const W &w) : v(x, y, z, w) {}

        template<i32 S, i32 L, typename A, typename R>
        explicit vec_class(const vec_class<S, L, A, void, R> &_v) : v(unitize(_v).val4()) {}

        template<i32 S, i32 L, typename A, i32 U, typename R>
        vec_class(const vec_class<S, L, A, vec_unit<U>, R> &_v) : v(_v.val4()) {}

        template<i32 S, i32 L, typename A, i32 U, typename R>
        vec_class &operator=(const vec_class<S, L, A, vec_unit<U>, R> &_v) { v = _v.val4(); return *this; }

        template<typename T> const f32 &operator[](T i) const { return v[i]; }

        typedef       vec_class<4, -1, void, vec_unit<1>, rep_std<true>> packed;
        typedef const vec_class<4, -1, void, vec_unit<1>, rep_constant>  constant;
    };

    template<typename U0>
    class vec_class<4, -1, void, U0, rep_constant> {

    public:
        f32 x_, y_, z_, w_;

        operator const vec_class<4, -1, void, U0>&() const { return reinterpret_cast<const vec_class<4, -1, void, U0>&>(*this); }

        const float4 &baseval() const { return reinterpret_cast<const float4&>(*this); }
        const float4 &val4()    const { return baseval(); }
        const float4 &val34()   const { return baseval(); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x_; }
        const get_y_type &get_y() const { return y_; }
        const get_z_type &get_z() const { return z_; }
        const get_w_type &get_w() const { return w_; }
    };

    template<>
    class vec_class<4, -1, void, void, rep_std<true>> { // vector4::packed

    protected:
        f32 x, y, z, w;

    public:
        explicit vec_class(const float4 &_v) { _v.store4((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load4((f32*)this);

            return result;
        }

        float4 val4()  const { return baseval(); }
        float4 val34() const { return baseval(); }

        operator const vec_class<4>() const { return vec_class<4>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
        const get_w_type &get_w() const { return w; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }
        template<typename V> void set_w(const V &val) { w = val; }

        vec_class() {}

        template<typename X, typename Y, typename Z, typename W>
        vec_class(const X &_x, const Y &_y, const Z &_z, const W &_w) : x(_x), y(_y), z(_z), w(_w) {}

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class(const vec_class<S, L, A, U, R> &_v) { _v.val4().store4((f32*)this); }

        template<typename U>
        vec_class &operator=(const vec_class<4, -1, void, U> &_v) { _v.val4().store4((f32*)this); return *this; }

        template<i32 S, i32 L, typename A, typename U, typename R>
        vec_class &operator=(const vec_class<S, L, A, U, R> &_v) { _v.val4().store4((f32*)this); return *this; }

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
        template<typename T>       f32 &operator[](T i)       { return ((f32*)this)[i]; }

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<4, -1, void, vec_unit<1>, rep_std<true>> { // unit_vector4::packed

    protected:
        f32 x, y, z, w;

    public:
        explicit vec_class(const float4 &_v) { _v.store4((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load4((f32*)this);

            return result;
        }

        float4 val4()  const { return baseval(); }
        float4 val34() const { return baseval(); }

        operator const vec_class<4, -1, void, vec_unit<1>>() const { return vec_class<4, -1, void, vec_unit<1>>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
        const get_w_type &get_w() const { return w; }

        vec_class() {}

        template<typename X, typename Y, typename Z, typename W>
        vec_class(const X &_x, const Y &_y, const Z &_z, const W &_w) : x(_x), y(_y), z(_z), w(_w) {}

        template<i32 S, i32 L, typename A, typename R>
        explicit vec_class(const vec_class<S, L, A, void, R> &_v) { *this = unitize(_v); }

        template<i32 S, i32 L, typename A, i32 U, typename R>
        vec_class(const vec_class<S, L, A, vec_unit<U>, R> &_v) { _v.val4().store4((f32*)this); }

        vec_class &operator=(const vec_class<4, -1, void, vec_unit<1>> &_v) { _v.val4().store4((f32*)this); return *this; }

        template<i32 S, i32 L, typename A, i32 U, typename R>
        vec_class &operator=(const vec_class<S, L, A, vec_unit<U>, R> &_v) { _v.val4().store4((f32*)this); return *this; }

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
    };

    template<>
    class vec_class<3, 0> { // dir3

    protected:
        float4 v;

    public:
        explicit vec_class(const float4 &_v) : v(_v) {}

        const float4 &baseval() const { return v; }

        float4 val4() const {
            float4 res = v;
            res.set_w(reinterpret_cast<const float1&>(float1_zero));

            return res;
        }

        const float4 &val34() const { return baseval(); }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float1             get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return float1_zero; }

        template<typename V> void set_x(const V &val) { v.set_x(val); }
        template<typename V> void set_y(const V &val) { v.set_y(val); }
        template<typename V> void set_z(const V &val) { v.set_z(val); }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &x, const Y &y, const Z &z) : v(x, y, z) {}

        template<typename A, typename U, typename R>
        vec_class(const vec_class<3, 0, A, U, R> &_v) : v(_v.val34()) {}

        template<i32 S, i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<S, L, A, U, R> &_v) : v(_v.val34()) {}

        template<typename A, typename U, typename R>
        vec_class &operator=(const vec_class<3, 0, A, U, R> &_v) { v = _v.val34(); return *this; }

        template<typename T> const f32 &operator[](T i) const { return v[i]; }
        template<typename T>       f32 &operator[](T i)       { return v[i]; }

        typedef vec_class<3, 0, void, void, rep_std<true>> packed;
        typedef const vec_class<3, 0, void, void, rep_constant> constant;

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<3, 1> { // position3

    protected:
        float4 v;

    public:
        explicit vec_class(const float4 &_v) : v(_v) {}

        const float4 &baseval() const { return v; }

        float4 val4() const {
            float4 res = v;
            res.set_w(reinterpret_cast<const float1&>(float1_one));

            return res;
        }

        const float4 &val34() const { return baseval(); }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float1             get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return float1_one; }

        template<typename V> void set_x(const V &val) { v.set_x(val); }
        template<typename V> void set_y(const V &val) { v.set_y(val); }
        template<typename V> void set_z(const V &val) { v.set_z(val); }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &x, const Y &y, const Z &z) : v(x, y, z) {}

        template<typename A, typename U, typename R>
        vec_class(const vec_class<3, 1, A, U, R> &_v) : v(_v.val34()) {}

        template<i32 S, i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<S, L, A, U, R> &_v) : v(_v.val34()) {}

        template<typename A, typename U, typename R>
        vec_class &operator=(const vec_class<3, 1, A, U, R> &_v) { v = _v.val34(); return *this; }

        template<typename T> const f32 &operator[](T i) const { return v[i]; }
        template<typename T>       f32 &operator[](T i)       { return v[i]; }

        typedef       vec_class<3, 1, void, void, rep_std<true>> packed;
        typedef const vec_class<3, 1, void, void, rep_constant>  constant;

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<3, 0, void, vec_unit<1>> { // unit_dir3

    protected:
        float4 v;

    public:
        explicit vec_class(const float4 &_v) : v(_v) {}

        const float4 &baseval() const { return v; }

        float4 val4() const {
            float4 res = v;
            res.set_w(reinterpret_cast<const float1&>(float1_zero));

            return res;
        }

        const float4 &val34() const { return baseval(); }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float1             get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return float1_zero; }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &x, const Y &y, const Z &z) : v(x, y, z) {}

        template<typename A, typename R>
        explicit vec_class(const vec_class<3, 0, A, void, R> &_v) : v(unitize(_v).val34()) {}

        template<typename A, i32 U, typename R>
        vec_class(const vec_class<3, 0, A, vec_unit<U>, R> &_v) : v(_v.val34()) {}

        template<typename A, i32 U, typename R>
        vec_class &operator=(const vec_class<3, 0, A, vec_unit<U>, R> &_v) { v = _v.val34(); return *this; }

        template<typename T> const f32 &operator[](T i) const { return v[i]; }

        typedef       vec_class<3, 0, void, vec_unit<1>, rep_std<true>> packed;
        typedef const vec_class<3, 0, void, vec_unit<1>, rep_constant>  constant;
    };

    template<typename U0>
    class vec_class<3, 0, void, U0, rep_constant> {

    public:
        f32 x_, y_, z_, padzerow_;

        operator const vec_class<3, 0, void, U0>&() const { return reinterpret_cast<const vec_class<3, 0, void, U0>&>(*this); }

        const float4 &baseval() const { return reinterpret_cast<const float4&>(*this); }
        const float4 &val4()    const { return baseval(); }
        const float4 &val34()   const { return baseval(); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return x_; }
        get_y_type get_y() const { return y_; }
        get_z_type get_z() const { return z_; }
        get_w_type get_w() const { return 0.0f; }
    };

    template<typename U0>
    class vec_class<3, 1, void, U0, rep_constant> {

    public:
        f32 x_, y_, z_, padzerow_;

        operator const vec_class<3, 1, void, U0>&() const { return reinterpret_cast<const vec_class<3, 1, void, U0>&>(*this); }

        const float4 &baseval() const { return reinterpret_cast<const float4&>(*this); }

        float4 val4() const {
            float4 res = baseval();
            res.set_w(reinterpret_cast<const float1&>(float1_one));

            return res;
        }

        const float4 &val34() const { return baseval(); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x_; }
        const get_y_type &get_y() const { return y_; }
        const get_z_type &get_z() const { return z_; }
              get_w_type  get_w() const { return 1.0f; }
    };

    template<>
    class vec_class<3, 0, void, void, rep_std<true>> { // dir3::packed

    protected:
        f32 x, y, z;

    public:
        explicit vec_class(const float4 &_v) { _v.store3((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load3((f32*)this);

            return result;
        }

        float4 val4() const {
            float4 res = baseval();
            res.set_w(reinterpret_cast<const float1&>(float1_zero));

            return res;
        }

        float4 val34() const { return baseval(); }

        operator const vec_class<3, 0>() const { return vec_class<3, 0>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
              get_w_type  get_w() const { return 0.0f; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &_x, const Y &_y, const Z &_z) : x(_x), y(_y), z(_z) {}

        template<typename A, typename U, typename R>
        vec_class(const vec_class<3, 0, A, U, R> &_v) { _v.val34().store3((f32*)this); }

        template<i32 S, i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<S, L, A, U, R> &_v) { _v.val34().store3((f32*)this); }

        template<typename A, typename U, typename R>
        vec_class &operator=(const vec_class<3, 0, A, U, R> &_v) { _v.val34().store3((f32*)this); return *this; }

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
        template<typename T>       f32 &operator[](T i)       { return ((f32*)this)[i]; }

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<3, 1, void, void, rep_std<true>> { // position3::packed

    protected:
        f32 x, y, z;

    public:
        explicit vec_class(const float4 &_v) { _v.store3((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load3((f32*)this);

            return result;
        }

        float4 val4() const {
            float4 res = baseval();
            res.set_w(reinterpret_cast<const float1&>(float1_one));

            return res;
        }

        float4 val34() const { return baseval(); }

        operator const vec_class<3, 1>() const { return vec_class<3, 1>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
              get_w_type  get_w() const { return 1.0f; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &_x, const Y &_y, const Z &_z) : x(_x), y(_y), z(_z) {}

        template<typename A, typename U, typename R>
        vec_class(const vec_class<3, 1, A, U, R> &_v) { _v.val34().store3((f32*)this); }

        template<i32 S, i32 L, typename A, typename U, typename R>
        explicit vec_class(const vec_class<S, L, A, U, R> &_v) { _v.val34().store3((f32*)this); }

        template<typename A, typename U, typename R>
        vec_class &operator=(const vec_class<3, 1, A, U, R> &_v) { _v.val34().store3(&x); return *this; }

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
        template<typename T>       f32 &operator[](T i)       { return ((f32*)this)[i]; }

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class vec_class<3, 0, void, vec_unit<1>, rep_std<true>> { // unit_dir3::packed

    protected:
        f32 x, y, z;

    public:
        explicit vec_class(const float4 &_v) { _v.store3((f32*)this); }

        float4 baseval() const {
            float4 result;
            result.load3((f32*)this);

            return result;
        }

        float4 val4() const {
            float4 res = baseval();
            res.set_w(reinterpret_cast<const float1&>(float1_zero));

            return res;
        }

        float4 val34() const { return baseval(); }

        operator const vec_class<3, 0, void, vec_unit<1>>() const { return vec_class<3, 0, void, vec_unit<1>>(baseval()); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return x; }
        const get_y_type &get_y() const { return y; }
        const get_z_type &get_z() const { return z; }
              get_w_type  get_w() const { return 0.0f; }

        vec_class() {}

        template<typename X, typename Y, typename Z>
        vec_class(const X &_x, const Y &_y, const Z &_z) : x(_x), y(_y), z(_z) {}

        template<typename A, typename R>
        explicit vec_class(const vec_class<3, 0, A, void, R> &_v) { *this = unitize(_v); }

        template<typename A, i32 U, typename R>
        vec_class(const vec_class<3, 0, A, vec_unit<U>, R> &_v) { _v.val34().store3((f32*)this); }

        template<typename A, i32 U, typename R>
        vec_class &operator=(const vec_class<3, 0, A, vec_unit<U>, R> &_v) { _v.val34().store3((f32*)this); return *this; }

        template<typename T> const f32 &operator[](T i) const { return ((const f32*)this)[i]; }
    };

    template<i32 S, i32 L, i32 A>
    class vec_class<S, L, vec_axis<A>, void, rep_constant> {

    public:
        f32 x_, padzeroy_, padzeroz_, padzerow_;

        operator const vec_class<S, L, vec_axis<A>>&() const { return reinterpret_cast<const vec_class<S, L, vec_axis<A>>&>(*this); }

        f32 baseval() const { return x_; }

        const float1_axis<A> &val4()  const { return reinterpret_cast<const float1_axis<A>&>(*this); }
        const float1_axis<A> &val34() const { return reinterpret_cast<const float1_axis<A>&>(*this); }

        f32 val1() const { return x_; }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return A == 0 ? x_ : 0.0f; }
        get_y_type get_y() const { return A == 1 ? x_ : 0.0f; }
        get_z_type get_z() const { return A == 2 ? x_ : 0.0f; }
        get_w_type get_w() const { return A == 3 ? x_ : 0.0f; }
    };

    template<i32 S, i32 L, i32 A>
    class vec_class<S, L, vec_axis<A>> {

    protected:
        float1 v;

    public:
        const float1 &baseval() const { return v; }

        const float1_axis<A> &val4()  const { return reinterpret_cast<const float1_axis<A>&>(*this); }
        const float1_axis<A> &val34() const { return reinterpret_cast<const float1_axis<A>&>(*this); }

        const float1 &val1() const { return v; }

        typedef float1 get_x_type;
        typedef float1 get_y_type;
        typedef float1 get_z_type;
        typedef float1 get_w_type;

        const get_x_type &get_x() const { return A == 0 ? v : reinterpret_cast<const float1&>(float1_zero); }
        const get_y_type &get_y() const { return A == 1 ? v : reinterpret_cast<const float1&>(float1_zero); }
        const get_z_type &get_z() const { return A == 2 ? v : reinterpret_cast<const float1&>(float1_zero); }
        const get_w_type &get_w() const { return A == 3 ? v : reinterpret_cast<const float1&>(float1_zero); }

        vec_class() {}

        template<typename X>
        explicit vec_class(const X &x) : v(x) {}

        template<typename U, typename R>
        vec_class(const vec_class<S, L, vec_axis<A>, U, R> &_v) : v(_v.val1()) {}

        template<typename U, typename R>
        vec_class &operator=(const vec_class<S, L, vec_axis<A>, U, R> &_v) { v = _v.val1(); return *this; }

        typedef const vec_class<S, L, vec_axis<A>, void, rep_constant> constant;

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<i32 S, i32 L, i32 A>
    class vec_class<S, L, vec_axis<A>, void, rep_type<f32>> {

    protected:
        f32 v;

    public:
        f32 baseval() const { return v; }

        float1_axis<A> val4()  const { return float1_axis<A>(float1(v)); }
        float1_axis<A> val34() const { return float1_axis<A>(float1(v)); }

        f32 val1() const { return v; }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return A == 0 ? v : 0.0f; }
        get_y_type get_y() const { return A == 1 ? v : 0.0f; }
        get_z_type get_z() const { return A == 2 ? v : 0.0f; }
        get_w_type get_w() const { return A == 3 ? v : 0.0f; }

        vec_class() {}

        template<typename X>
        explicit vec_class(const X &x) : v(x) {}

        template<typename U, typename R>
        vec_class(const vec_class<S, L, vec_axis<A>, U, R> &_v) : v(_v.val1()) {}

        template<typename U, typename R>
        vec_class &operator=(const vec_class<S, L, vec_axis<A>, U, R> &_v) { v = _v.val1(); return *this; }

        typedef const vec_class<S, L, vec_axis<A>, void, rep_constant> constant;

        template<typename T> vec_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> vec_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> vec_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> vec_class &operator/=(const T &a) { return *this = *this / a; }
    };

    inline const float4::constant float4_zzzn = { 0.0f, 0.0f, 0.0f, -1.0f };

    template<i32 A, i32 U>
    const float4 &unit_axis_val4() {
        if (U == 1)
            return A == 0 ? reinterpret_cast<const float4&>(f4_1000) :
                   A == 1 ? reinterpret_cast<const float4&>(f4_0100) :
                   A == 2 ? reinterpret_cast<const float4&>(f4_0010) :
                            reinterpret_cast<const float4&>(f4_0001);

        return A == 0 ? reinterpret_cast<const float4&>(axis180_negx) :
               A == 1 ? reinterpret_cast<const float4&>(axis180_negy) :
               A == 2 ? reinterpret_cast<const float4&>(axis180_negz) :
                        reinterpret_cast<const float4&>(float4_zzzn);
    }

    template<i32 S, i32 L, i32 A, i32 U>
    class vec_class<S, L, vec_axis<A>, vec_unit<U>> {

    public:
        const float4 &val4()  const { return unit_axis_val4<A, U>(); }
        const float4 &val34() const { return unit_axis_val4<A, U>(); }

        const float1 &val1() const { return compile_time_if<(U == 1)>(reinterpret_cast<const float1&>(float1_one), reinterpret_cast<const float1&>(float1_neg_one)); }

        typedef float1 get_x_type;
        typedef float1 get_y_type;
        typedef float1 get_z_type;
        typedef float1 get_w_type;

        const get_x_type &get_x() const { return compile_time_if<(A == 0)>(val1(), reinterpret_cast<const float1&>(float1_zero)); }
        const get_y_type &get_y() const { return compile_time_if<(A == 1)>(val1(), reinterpret_cast<const float1&>(float1_zero)); }
        const get_z_type &get_z() const { return compile_time_if<(A == 2)>(val1(), reinterpret_cast<const float1&>(float1_zero)); }
        const get_w_type &get_w() const { return compile_time_if<(A == 3)>(val1(), reinterpret_cast<const float1&>(float1_zero)); }

        vec_class() {}
    };

    template<i32 n, i32 S, i32 L, typename A, typename U, typename R>
    typename get_set<n, vec_class<S, L, A, U, R>>::type get(const vec_class<S, L, A, U, R> &a) {
        return get_set<n, vec_class<S, L, A, U, R>>::get(a);
    }

    template<i32 n, i32 S, i32 L, typename A, typename U, typename R, typename V>
    void set(vec_class<S, L, A, U, R> &a, const V &v) {
        get_set<n, vec_class<S, L, A, U, R>>::set(a, v);
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A, U> operator-(const vec_class<S, L, A, U, R> &a) {
        return vec_class<S, L, A, U>(-a.baseval());
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    class result_type<op_neg, vec_class<S, L, A, U, R>> {

    public:
        typedef vec_class<S, L, A, U> type;
    };

    template<i32 S, i32 L, i32 A, i32 U, typename R>
    vec_class<S, L, vec_axis<A>, vec_unit<(0 - U)>, R> operator-(const vec_class<S, L, vec_axis<A>, vec_unit<U>, R>&) {
        return vec_class<S, L, vec_axis<A>, vec_unit<(0 - U)>, R>();
    }

    template<i32 S, i32 L, i32 A, i32 U, typename R>
    class result_type<op_neg, vec_class<S, L, vec_axis<A>, vec_unit<U>, R>> {

    public:
        typedef vec_class<S, L, vec_axis<A>, vec_unit<(0 - U)>, R> type;
    };

    template<i32 S, i32 L, typename R>
    vec_class<S, L, vec_axis_zero, void, R> operator-(const vec_class<S, L, vec_axis_zero, void, R>&) {
        return vec_class<S, L, vec_axis_zero, void, R>();
    }

    template<i32 S, i32 L, typename R>
    class result_type<op_neg, vec_class<S, L, vec_axis_zero, void, R>> {

    public:
        typedef vec_class<S, L, vec_axis_zero, void, R> type;
    };

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A, U> inv(const vec_class<S, L, A, U, R> &a) {
        return vec_class<S, L, A, U>(inv(a.baseval()));
    }

    template<i32 L, typename R>
    vec_class<3, L> inv(const vec_class<3, L, void, void, R> &a) {
        return vec_class<3, L>(inv3(a.baseval()));
    }

    template<i32 S, i32 L, typename A, i32 U, typename R>
    const vec_class<S, L, A, vec_unit<U>, R> &inv(const vec_class<S, L, A, vec_unit<U>, R> &a) {
        return a;
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A, U> operator~(const vec_class<S, L, A, U, R> &a) {
        return inv(a);
    }

    template<i32 S, i32 L, typename A, i32 U, typename R>
    const vec_class<S, L, A, vec_unit<U>, R> &operator~(const vec_class<S, L, A, vec_unit<U>, R> &a) {
        return a;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 operator+(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return vector4(float4(a.val4()) + float4(b.val4()));
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    vector4 operator+(const vec_class<S1, L1, A1, U1, R1> &a, const vector4_zero&) {
        return vector4(a);
    }

    template<i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 operator+(const vector4_zero&, const vec_class<S2, L2, A2, U2, R2> &b) {
        return vector4(b);
    }

    template<i32 S1, i32 L1>
    vector4_zero operator+(const vec_class<S1, L1, vec_axis_zero> &, const vector4_zero&) {
        return vector4_zero();
    }

    template<i32 S1, i32 L1>
    vector4_zero operator+(const vector4_zero&, const vec_class<S1, L1, vec_axis_zero>&) {
        return vector4_zero();
    }

    inline vector4_zero operator+(const vector4_zero&, const vector4_zero&) {
        return vector4_zero();
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    vec_class<3, (L1 | L2)> operator+(const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return vec_class<3, (L1 | L2)>(float4(a.val34()) + float4(b.val34()));
    }

    template<typename A1, typename U1, typename R1>
    vec_class<3, 1, A1, U1, R1> operator+(const vec_class<3, 1, A1, U1, R1> &a, const position3_zero&) {
        return a;
    }

    template<typename A1, typename U1, typename R1>
    vec_class<3, 1, A1, U1, R1> operator+(const position3_zero&, const vec_class<3, 1, A1, U1, R1> &a) {
        return a;
    }

    template<typename A1, typename U1, typename R1>
    position3 operator+(const vec_class<3, 0, A1, U1, R1> &a, const position3_zero&) {
        return position3(a);
    }

    template<typename A1, typename U1, typename R1>
    position3 operator+(const position3_zero&, const vec_class<3, 0, A1, U1, R1> &a) {
        return position3(a);
    }

    template<i32 L1, i32 L2>
    vec_class<3, (L1 | L2), vec_axis_zero> operator+(const vec_class<3, L1, vec_axis_zero>&,
                                                     const vec_class<3, L2, vec_axis_zero>&) {

        return vec_class<3, (L1 | L2), vec_axis_zero>();
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 operator-(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return vector4(float4(a.val4()) - float4(b.val4()));
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    bool operator==(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return float4(a.val4()) == float4(b.val4());
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    bool operator!=(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return float4(a.val4()) != float4(b.val4());
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    vec_class<3, (L1 ^ L2)> operator-(const vec_class<3, L1, A1, U1, R1> &a,
                                      const vec_class<3, L2, A2, U2, R2> &b) {

        return vec_class<3, (L1 ^ L2)>(float4(a.val34()) - float4(b.val34()));
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    bool operator==(const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return compare_all_eq<0x7>(float4(a.val34()), float4(b.val34()));
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    bool operator!=(const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return compare_any_ne<0x7>(float4(a.val34()), float4(b.val34()));
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    vector4 operator-(const vec_class<S1, L1, A1, U1, R1> &a, const vector4_zero&) {
        return vector4(a);
    }

    template<i32 S1, i32 L1>
    vector4_zero operator-(const vec_class<S1, L1, vec_axis_zero>&, const vector4_zero&) {
        return vector4_zero();
    }

    template<i32 L1, typename A1, typename U1, typename R1>
    vec_class<3, (1 - L1)> operator-(const vec_class<3, L1, A1, U1, R1> &a, const position3_zero&) {
        return vec_class<3, (1 - L1)>(a);
    }

    template<i32 L1>
    vec_class<3, (1 - L1), vec_axis_zero> operator-(const vec_class<3, L1, vec_axis_zero>&,
                                                    const position3_zero&) {

        return vec_class<3, (1 - L1), vec_axis_zero>();
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 min(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return vector4(min(float4(a.val4()), float4(b.val4())));
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    vec_class<3, (L1 & L2)> min(const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return vec_class<3, (L1 & L2)>(min(float4(a.val34()), float4(b.val34())));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> min(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L>(min(float4(a.val34()), b));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> min(f32 b, const vec_class<S, L, A, U, R> &a) {
        return vec_class<S, L>(min(float4(a.val34()), b));
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 max(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return vector4(max(float4(a.val4()), float4(b.val4())));
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    vec_class<3, (L1 | L2)> max(const vec_class<3, L1, A1, U1, R1> &a,
                                const vec_class<3, L2, A2, U2, R2> &b) {

        return vec_class<3, (L1 | L2)>(max(float4(a.val34()), float4(b.val34())));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> max(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L>(max(float4(a.val34()), b));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> max(f32 b, const vec_class<S, L, A, U, R> &a) {
        return vec_class<S, L>(max(float4(a.val34()), b));
    }

    template<i32 S1, i32 L1, typename U1, typename R1,
             i32 S2, i32 L2, typename U2, typename R2>
    float1 operator*(const vec_class<S1, L1, void, U1, R1> &a,
                     const vec_class<S2, L2, void, U2, R2> &b) {

        return dot4(float4(a.val4()), float4(b.val4()));
    }

    template<typename U1, typename R1,
             typename U2, typename R2>
    float1 operator*(const vec_class<3, 1, void, U1, R1> &a, const vec_class<4, -1, void, U2, R2> &b) {
        return dot34(float4(a.val34()), float4(b.val4()));
    }

    template<typename U1, typename R1,
             typename U2, typename R2>
    float1 operator*(const vec_class<4, -1, void, U1, R1> &a, const vec_class<3, 1, void, U2, R2> &b) {
        return dot34(float4(b.val34()), float4(a.val4()));
    }

    template<typename U1, typename R1,
             typename U2, typename R2>
    float1 operator*(const vec_class<3, 0, void, U1, R1> &a, const vec_class<4, -1, void, U2, R2> &b) {
        return dot3(float4(a.val34()), float4(b.val4()));
    }

    template<typename A1, typename U1, typename R1,
             typename A2, typename U2, typename R2>
    float1 operator*(const vec_class<4, -1, A1, U1, R1> &a, const vec_class<3, 0, A2, U2, R2> &b) {
        return dot3(float4(a.val34()), float4(b.val34()));
    }

    // sub_4020C0
    template<i32 L1, typename U1, typename R1,
             i32 L2, typename U2, typename R2>
    float1 operator*(const vec_class<3, L1, void, U1, R1> &a, const vec_class<3, L2, void, U2, R2> &b) {
        return dot3(float4(a.val34()), float4(b.val34()));
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename R2>
    float1 operator*(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, vec_axis<A2>, void, R2> &b) {
        return get<A2>(a) * b.val1();
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename R2>
    typename get_set<A2, vec_class<S1, L1, A1, U1, R1>>::type
    operator*(const vec_class<S1, L1, A1, U1, R1> &a,const vec_class<S2, L2, vec_axis<A2>, vec_unit<1>, R2>&) {
        return get<A2>(a);
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename R2>
    typename get_set<A2, vec_class<S1, L1, A1, U1, R1>>::type
    operator*(const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, vec_axis<A2>, vec_unit<-1>, R2>&) {
        return -get<A2>(a);
    }

    template<typename A1, typename U1, typename R1>
    typename vec_class<4, -1, A1, U1, R1>::get_w_type operator*(const vec_class<4, -1, A1, U1, R1> &a, const position3_zero&) {
        return a.get_w();
    }

    template<i32 S1, i32 L1, i32 A1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    float1 operator*(const vec_class<S1, L1, vec_axis<A1>, void, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return a.val1() * get<A1>(b);
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename R2>
    typename get_set<A2, vec_class<S1, L1, A1, U1, R1>>::type
    operator*(const vec_class<S2, L2, vec_axis<A2>, vec_unit<1>, R2>&, const vec_class<S1, L1, A1, U1, R1> &a) {
        return get<A2>(a);
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename R2>
    typename get_set<A2, vec_class<S1, L1, A1, U1, R1>>::type
    operator*(const vec_class<S2, L2, vec_axis<A2>, vec_unit<-1>, R2>&, const vec_class<S1, L1, A1, U1, R1> &a) {
        return -get<A2>(a);
    }

    template<typename A1, typename U1, typename R1>
    typename vec_class<4, -1, A1, U1, R1>::get_w_type
    operator*(const position3_zero&, const vec_class<4, -1, A1, U1, R1> &a) {
        return a.get_w();
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const vec_class<S1, L1, A1, U1, R1>&, const vector4_zero&) {
        return float1_zero;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const vec_class<S1, L1, A1, U1, R1>&, const dir3_zero&) {
        return float1_zero;
    }

    template<i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const vec_class<3, L1, A1, U1, R1>&, const position3_zero&) {
        return float1_zero;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const vector4_zero&, const vec_class<S1, L1, A1, U1, R1>&) {
        return float1_zero;
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const dir3_zero&, const vec_class<S1, L1, A1, U1, R1>&) {
        return float1_zero;
    }

    template<i32 L1, typename A1, typename U1, typename R1>
    const float1 &operator*(const position3_zero&, const vec_class<3, L1, A1, U1, R1>&) {
        return float1_zero;
    }

    template<i32 S1, i32 L1, i32 S2, i32 L2>
    const float1 &operator*(const vec_class<S1, L1, vec_axis_zero>&, const vec_class<S2, L2, vec_axis_zero>&) {
        return float1_zero;
    }

    inline const float1 &operator*(const unit_dir_w&,     const position3_zero&) { return float1_one; }
    inline const float1 &operator*(const position3_zero&, const unit_dir_w&)     { return float1_one; }
    inline const float1 &operator*(const unit_neg_dir_w&, const position3_zero&) { return float1_neg_one; }
    inline const float1 &operator*(const position3_zero&, const unit_neg_dir_w&) { return float1_neg_one; }

    template<i32 S, i32 L, i32 A, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, void, R1> &a,
                     const vec_class<S, L, vec_axis<A>, void, R2> &b) {

        return a.val1() * b.val1();
    }

    template<i32 S, i32 L, i32 A, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, void, R1> &a,
                     const vec_class<S, L, vec_axis<A>, vec_unit<1>, R2>&) {

        return a.val1();
    }

    template<i32 S, i32 L, i32 A, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, void, R1> &a,
                     const vec_class<S, L, vec_axis<A>, vec_unit<-1>, R2>&) {

        return -a.val1();
    }

    template<i32 S, i32 L, i32 A, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, vec_unit<1>, R1>&,
                     const vec_class<S, L, vec_axis<A>, void, R2> &b) {

        return b.val1();
    }

    template<i32 S, i32 L, i32 A, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, vec_unit<-1>, R1>&,
                     const vec_class<S, L, vec_axis<A>, void, R2> &b) {

        return -b.val1();
    }

    template<i32 S, i32 L, i32 A, i32 U1, i32 U2, typename R1, typename R2>
    float1 operator*(const vec_class<S, L, vec_axis<A>, vec_unit<U1>, R1>&,
                     const vec_class<S, L, vec_axis<A>, vec_unit<U2>, R2>&) {

        return (f32)(U1 * U2);
    }

    template<i32 S1, i32 L1, i32 A1, typename U1, typename R1,
             i32 S2, i32 L2, i32 A2, typename U2, typename R2>
    float1 operator*(const vec_class<S1, L1, vec_axis<A1>, U1, R1>&,
                     const vec_class<S2, L2, vec_axis<A2>, U2, R2>&) {

        return reinterpret_cast<const float1&>(float1_zero);
    }

    template<typename A1, typename U1, typename R1,
             typename A2, typename U2, typename R2>
    dir3 cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, A2, U2, R2> &b) {
        return dir3(fast_cross3(float4(a.val34()), float4(b.val34())));
    }

    template<typename A1, typename U1, typename R1>
    dir3_zero cross(const vec_class<3, 0, A1, U1, R1>&, const dir3_zero&) {
        return dir3_zero();
    }

    template<typename A1, typename U1, typename R1>
    dir3_zero cross(const dir3_zero&, const vec_class<3, 0, A1, U1, R1>&) {
        return dir3_zero();
    }

    inline dir3_zero cross(const dir3_zero&, const dir3_zero&) {
        return dir3_zero();
    }

    template<i32 A, typename U1, typename R1,
                    typename U2, typename R2>
    dir3_zero cross(const vec_class<3, 0, vec_axis<A>, U1, R1>&, const vec_class<3, 0, vec_axis<A>, U2, R2>&) {
        return dir3_zero();
    }

    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, void, R2> &b) { return dir_z(a.val1() * b.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, void, R2> &b) { return dir_x(a.val1() * b.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, void, R2> &b) { return dir_y(a.val1() * b.val1()); }
    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, void, R2> &b) { return dir_z(-(a.val1() * b.val1())); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, void, R2> &b) { return dir_x(-(a.val1() * b.val1())); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, void, R2> &b) { return dir_y(-(a.val1() * b.val1())); }

    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&) { return dir_z(a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&) { return dir_x(a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&) { return dir_y(a.val1()); }
    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&) { return dir_z(-a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&) { return dir_x(-a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&) { return dir_y(-a.val1()); }

    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&) { return dir_z(-a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&) { return dir_x(-a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&) { return dir_y(-a.val1()); }
    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<1>, void, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&) { return dir_z(a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<2>, void, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&) { return dir_x(a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<0>, void, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&) { return dir_y(a.val1()); }

    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<0>, void, R1> &a) { return dir_z(-a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<1>, void, R1> &a) { return dir_x(-a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<2>, void, R1> &a) { return dir_y(-a.val1()); }
    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<1>, void, R1> &a) { return dir_z(a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<2>, void, R1> &a) { return dir_x(a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&, const vec_class<3, 0, vec_axis<0>, void, R1> &a) { return dir_y(a.val1()); }

    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<0>, void, R1> &a) { return dir_z(a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<1>, void, R1> &a) { return dir_x(a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<2>, void, R1> &a) { return dir_y(a.val1()); }
    template<typename R1, typename R2> dir_z
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<1>, void, R1> &a) { return dir_z(-a.val1()); }
    template<typename R1, typename R2> dir_x
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<2>, void, R1> &a) { return dir_x(-a.val1()); }
    template<typename R1, typename R2> dir_y
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&, const vec_class<3, 0, vec_axis<0>, void, R1> &a) { return dir_y(-a.val1()); }

    template<i32 A, i32 U1, i32 U2, typename R1, typename R2>
    dir3_zero cross(const vec_class<3, 0, vec_axis<A>, vec_unit<U1>, R2>&, const vec_class<3, 0, vec_axis<A>, vec_unit<U2>, R1>&) {
        return dir3_zero();
    }

    template<i32 A1, i32 A2, i32 U1, i32 U2, typename R1, typename R2>
    vec_class<3, 0, vec_axis<3 - A1 - A2>, vec_unit<U1 * U2 * (((A1 + 1) % 3) == A2 ? 1 : -1)>>
    cross(const vec_class<3, 0, vec_axis<A1>, vec_unit<U1>, R2>&,
          const vec_class<3, 0, vec_axis<A2>, vec_unit<U2>, R1>&) {

        return vec_class<3, 0, vec_axis<3 - A1 - A2>, vec_unit<U1 * U2 * (((A1 + 1) % 3) == A2 ? 1 : -1)>>();
    }

    inline const float4::constant cross_zpnz = {  0.0f,  1.0f, -1.0f, 0.0f };
    inline const float4::constant cross_nzpz = { -1.0f,  0.0f,  1.0f, 0.0f };
    inline const float4::constant cross_pnzz = {  1.0f, -1.0f,  0.0f, 0.0f };
    inline const float4::constant cross_znpz = {  0.0f, -1.0f,  1.0f, 0.0f };
    inline const float4::constant cross_pznz = {  1.0f,  0.0f, -1.0f, 0.0f };
    inline const float4::constant cross_npzz = { -1.0f,  1.0f,  0.0f, 0.0f };

    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<0>, void, R2> &b) {
        return dir3(swizzle<0, 2, 1, 3>(float4(a.val34())) * b.val1() * reinterpret_cast<const float4&>(cross_zpnz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<1>, void, R2> &b) {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * b.val1() * reinterpret_cast<const float4&>(cross_nzpz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<2>, void, R2> &b) {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * b.val1() * reinterpret_cast<const float4&>(cross_pnzz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<0>, void, R2> &a, const vec_class<3, 0, A1, U1, R1> &b) {
        return dir3(swizzle<0, 2, 1, 3>(float4(b.val34())) * a.val1() * reinterpret_cast<const float4&>(cross_znpz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<1>, void, R2> &a, const vec_class<3, 0, A1, U1, R1> &b) {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * b.val1() * reinterpret_cast<const float4&>(cross_pznz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<2>, void, R2> &a, const vec_class<3, 0, A1, U1, R1> &b) {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * b.val1() * reinterpret_cast<const float4&>(cross_npzz));
    }

    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&)  {
        return dir3(swizzle<0, 2, 1, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_zpnz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&) {
        return dir3(swizzle<0, 2, 1, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_znpz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<1>, R2>&,  const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<0, 2, 1, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_znpz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<0>, vec_unit<-1>, R2>&, const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<0, 2, 1, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_zpnz));
    }

    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&)  {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_nzpz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&) {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_pznz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<1>, R2>&,  const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_pznz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<1>, vec_unit<-1>, R2>&, const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<2, 1, 0, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_nzpz));
    }

    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&)  {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_pnzz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, A1, U1, R1> &a, const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&) {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_npzz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<1>, R2>&,  const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_npzz));
    }
    template<typename A1, typename U1, typename R1, typename R2> dir3
    cross(const vec_class<3, 0, vec_axis<2>, vec_unit<-1>, R2>&, const vec_class<3, 0, A1, U1, R1> &a) {
        return dir3(swizzle<1, 0, 2, 3>(float4(a.val34())) * reinterpret_cast<const float4&>(cross_pnzz));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A> operator*(f32 a, const vec_class<S, L, A, U, R> &b) {
        return vec_class<S, L, A>(a * b.baseval());
    }

    template<i32 S, i32 L>
    vec_class<S, L, vec_axis_zero> operator*(f32, const vec_class<S, L, vec_axis_zero>&) {
        return vec_class<S, L, vec_axis_zero>();
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator*(f32 a, const vec_class<S, L, vec_axis<A>, vec_unit<1>, R>&) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(a);
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator*(f32 a, const vec_class<S, L, vec_axis<A>, vec_unit<-1>, R>&) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(-a);
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A> operator*(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L, A>(a.baseval() * b);
    }

    template<i32 S, i32 L>
    vec_class<S, L, vec_axis_zero> operator*(const vec_class<S, L, vec_axis_zero>&, f32) {
        return vec_class<S, L, vec_axis_zero>();
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator*(const vec_class<S, L, vec_axis<A>, vec_unit<1>, R>&, f32 a) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(a);
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator*(const vec_class<S, L, vec_axis<A>, vec_unit<-1>, R>&, f32 a) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(-a);
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L, A> operator/(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L, A>(a.baseval() / b);
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator/(const vec_class<S, L, vec_axis<A>, vec_unit<1>, R>&, f32 a) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(inv_safe(a));
    }

    template<i32 S, i32 L, i32 A, typename R>
    vec_class<S, L, vec_axis<A>, void, rep_type<float1>> operator/(const vec_class<S, L, vec_axis<A>, vec_unit<-1>, R>&, f32 a) {
        return vec_class<S, L, vec_axis<A>, void, rep_type<float1>>(-inv_safe(a));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> operator+(f32 a, const vec_class<S, L, A, U, R> &b) {
        return vec_class<S, L>(a + float4(b.val34()));
    }

    template<i32 S, i32 L>
    vec_class<S, L> operator+(f32 a, const vec_class<S, L, vec_axis_zero>&) {
        return vec_class<S, L>(float4(a));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> operator+(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L>(float4(a.val34()) + b);
    }

    template<i32 S, i32 L>
    vec_class<S, L> operator+(const vec_class<S, L, vec_axis_zero>&, f32 b) {
        return vec_class<S, L>(float4(b));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    vec_class<S, L> operator-(const vec_class<S, L, A, U, R> &a, f32 b) {
        return vec_class<S, L>(float4(a.val34()) - b);
    }

    template<i32 S, i32 L>
    vec_class<S, L> operator-(const vec_class<S, L, vec_axis_zero>&, f32 b) {
        return vec_class<S, L>(float4(-b));
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    float1 abs_squared(const vec_class<S, L, A, U, R> &a) {
        return a * a;
    }

    template<i32 S, i32 L, typename A, typename U, typename R>
    float1 abs(const vec_class<S, L, A, U, R> &a) {
        return sqrt(a * a);
    }

    template<typename U, typename R>
    const vec_class<4, -1, void, vec_unit<1>, R> &declare_unit(const vec_class<4, -1, void, U, R> &a) {
        return reinterpret_cast<const vec_class<4, -1, void, vec_unit<1>, R>&>(a);
    }

    template<typename U, typename R>
    vec_class<4, -1, void, vec_unit<1>, R> unitize(const vec_class<4, -1, void, U, R> &a) {
        return declare_unit(a * rsqrt_safe(abs_squared(a)));
    }

    template<typename U, typename R>
    vec_class<4, -1, void, vec_unit<1>, R> unitize(const vec_class<4, -1, void, U, R> &a, float1 &mag) {
        mag = sqrt(abs_squared(a));

        return declare_unit(a * inv_safe(mag));
    }

    template<typename R>
    vec_class<4, -1, vec_axis<3>, vec_unit<1>> unitize(const vec_class<4, -1, vec_axis<3>, void, R>&) {
        return vec_class<4, -1, vec_axis<3>, vec_unit<1>>();
    }

    template<typename R>
    vec_class<4, -1, vec_axis<3>, vec_unit<1>> unitize(const vec_class<4, -1, vec_axis<3>, void, R> &a, float1 &mag) {
        mag = a.val1();

        return vec_class<4, -1, vec_axis<3>, vec_unit<1>>();
    }

    template<i32 U, typename R>
    vec_class<4, -1, vec_axis<3>, vec_unit<U>> unitize(const vec_class<4, -1, vec_axis<3>, vec_unit<U>, R>&) {
        return vec_class<4, -1, vec_axis<3>, vec_unit<U>>();
    }

    template<i32 U, typename R>
    vec_class<4, -1, vec_axis<3>, vec_unit<U>> unitize(const vec_class<4, -1, vec_axis<3>, vec_unit<U>, R>&, float1 &mag) {
        mag = float1_one;

        return vec_class<4, -1, vec_axis<3>, vec_unit<U>>();
    }

    template<typename U, typename R>
    const vec_class<3, 0, void, vec_unit<1>, R> &declare_unit(const vec_class<3, 0, void, U, R> &a) {
        return reinterpret_cast<const vec_class<3, 0, void, vec_unit<1>, R>&>(a);
    }

    template<typename U, typename R>
    vec_class<3, 0, void, vec_unit<1>, R> unitize(const vec_class<3, 0, void, U, R> &a) {
        return declare_unit(a * rsqrt_safe(abs_squared(a)));
    }

    template<typename U, typename R>
    vec_class<3, 0, void, vec_unit<1>, R> unitize(const vec_class<3, 0, void, U, R> &a, float1 &mag) {
        mag = sqrt(abs_squared(a));

        return declare_unit(a * inv_safe(mag));
    }

    template<i32 A, typename R>
    vec_class<3, 0, vec_axis<A>, vec_unit<1>> unitize(const vec_class<3, 0, vec_axis<A>, void, R>&) {
        return vec_class<3, 0, vec_axis<A>, vec_unit<1>>();
    }

    template<i32 A, typename R>
    vec_class<3, 0, vec_axis<A>, vec_unit<1>> unitize(const vec_class<3, 0, vec_axis<A>, void, R> &a, float1 &mag) {
        mag = a.val1();

        return vec_class<3, 0, vec_axis<A>, vec_unit<1>>();
    }

    template<i32 A, i32 U, typename R>
    vec_class<3, 0, vec_axis<A>, vec_unit<U>> unitize(const vec_class<3, 0, vec_axis<A>, vec_unit<U>, R>&) {
        return vec_class<3, 0, vec_axis<A>, vec_unit<U>>();
    }

    template<i32 A, i32 U, typename R>
    vec_class<3, 0, vec_axis<A>, vec_unit<U>> unitize(const vec_class<3, 0, vec_axis<A>, vec_unit<U>, R>&, float1 &mag) {
        mag = float1_one;

        return vec_class<3, 0, vec_axis<A>, vec_unit<U>>();
    }

    template<i32 S1, i32 L1, typename A1, typename U1, typename R1,
             i32 S2, i32 L2, typename A2, typename U2, typename R2>
    vector4 select(bool cond, const vec_class<S1, L1, A1, U1, R1> &a, const vec_class<S2, L2, A2, U2, R2> &b) {
        return cond ? (const vector4)a : (const vector4)b;
    }

    template<i32 L1, typename A1, typename U1, typename R1,
             i32 L2, typename A2, typename U2, typename R2>
    vec_class<3, (L1 | L2)> select(bool cond, const vec_class<3, L1, A1, U1, R1> &a, const vec_class<3, L2, A2, U2, R2> &b) {
        return cond ? (const vec_class<3, (L1 | L2)>)a : (const vec_class<3, (L1 | L2)>)b;
    }

    template<i32 S1, i32 L1, typename A1, i32 U1, typename R1,
             i32 S2, i32 L2, typename A2, i32 U2, typename R2>
    vec_class<((S1 > S2) ? S1 : S2), ((S1 > S2) ? L1 : L2), void, vec_unit<1>>
        select(bool cond, const vec_class<S1, L1, A1, vec_unit<U1>, R1> &a, const vec_class<S2, L2, A2, vec_unit<U2>, R2> &b) {

        typedef vec_class<((S1 > S2) ? S1 : S2), ((S1 > S2) ? L1 : L2), void, vec_unit<1>> unit_type;

        return cond ? (const unit_type)a : (const unit_type)b;
    }

    template<typename A1, typename R1,
             typename A2, i32 U2, typename R2>
    dir3 reflect(const vec_class<3, 0, A1, void, R1> &a, const vec_class<3, 0, A2, vec_unit<U2>, R2> &n) {
        return a - 2.0f * (a * n) * n;
    }

    template<typename A1, i32 U1, typename R1,
             typename A2, i32 U2, typename R2>
    unit_dir3 reflect(const vec_class<3, 0, A1, vec_unit<U1>, R1> &a, const vec_class<3, 0, A2, vec_unit<U2>, R2> &n) {
        return declare_unit(a - 2.0f * (a * n) * n);
    }

    template<typename A1, typename R1,
             typename A2, typename R2>
    dir3 reflect(const vec_class<3, 0, A1, void, R1> &a, const vec_class<3, 0, A2, void, R2> &n) {
        return a - 2.0f * (a * n) / (n * n) * n;
    }

    template<typename A1, i32 U1, typename R1,
             typename A2, typename R2>
    unit_dir3 reflect(const vec_class<3, 0, A1, vec_unit<U1>, R1> &a, const vec_class<3, 0, A2, void, R2> &n) {
        return declare_unit(a - 2.0f * (a * n) / (n * n) * n);
    }

    ASSERT_SIZEOF(vector4,           0x10);
    ASSERT_SIZEOF(vector4::packed,   0x10);
    ASSERT_SIZEOF(position3,         0x10);
    ASSERT_SIZEOF(position3::packed, 0x0C);
    ASSERT_SIZEOF(dir3,              0x10);
    ASSERT_SIZEOF(dir3::packed,      0x0C);
    ASSERT_SIZEOF(unit_dir3,         0x10);
}} // treyarch::math
