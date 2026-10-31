#pragma once

#include "treyarch/math/vector.hh"

namespace treyarch { namespace math {
    template<typename axis = void, bool unit = false, typename rep = rep_std<>> class quat_class;

    typedef quat_class<>           quaternion;
    typedef quat_class<void, true> unit_quaternion;

    typedef const quaternion      quaternion_arg;
    typedef const unit_quaternion unit_quaternion_arg;

    template<>
    class quat_class<> { // quaternion

    protected:
        float4 v;

    public:
        explicit quat_class(const float4 &_v) : v(_v) {}

        const float4 &val() const { return v; }

        quat_class() {}
        quat_class(f32 r) : v(0.0f, 0.0f, 0.0f, r) {}

        template<typename A1, typename U1, typename R1, typename W>
        quat_class(const vec_class<3, 0, A1, U1, R1> &xyz, const W &w) : v(xyz.get_x(), xyz.get_y(), xyz.get_z(), w) {}

        template<typename X, typename Y, typename Z, typename W>
        quat_class(const X &x, const Y &y, const Z &z, const W &w) : v(x, y, z, w) {}

        template<typename A1, bool U1, typename R1>
        quat_class(const quat_class<A1, U1, R1> &a) : v(a.val()) {}

        template<typename T> const f32 &operator[](T i) const { return v[i]; }
        template<typename T>       f32 &operator[](T i)       { return v[i]; }

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

        typedef dir3               imag_type;
        typedef float4::get_w_type real_type;

        const imag_type &get_imag() const { return reinterpret_cast<const imag_type&>(v); }
        const real_type &get_real() const { return v.get_w(); }

        template<typename V> void set_real(const V &val) { v.set_w(val); }

        typedef quat_class<void, false, rep_std<true>> packed;
        typedef const quat_class<void, false, rep_constant> constant;

        template<typename A1, bool U1, typename R1>
        quat_class &operator=(const quat_class<A1, U1, R1> &a) { v = a.val(); return *this; }

        template<typename T> quat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> quat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> quat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> quat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class quat_class<void, false, rep_std<true>> { // quaternion::packed

    protected:
        f32 x, y, z, w;

    public:
        explicit quat_class(const float4 &_v) { _v.store4((f32*)this); }

        float4 val() const {
            float4 result;
            result.load4((f32*)this);

            return result;
        }

        quat_class() {}
        quat_class(f32 r) : x(0.0f), y(0.0f), z(0.0f), w(r) {}

        template<typename A1, typename U1, typename R1, typename W>
        quat_class(const vec_class<3, 0, A1, U1, R1> &xyz, const W &_w) : w(_w) {
            float4(xyz.val34()).store3((f32*)this);
        }

        template<typename X, typename Y, typename Z, typename W>
        quat_class(const X &_x, const Y &_y, const Z &_z, const W &_w) : x(_x),
                                                                         y(_y),
                                                                         z(_z),
                                                                         w(_w) {}

        template<typename A1, bool U1, typename R1>
        quat_class(const quat_class<A1, U1, R1> &a) { a.val().store4((f32*)this); }

        template<typename T> const f32 &operator[](T i) const { return (&x)[i]; }
        template<typename T>       f32 &operator[](T i)       { return (&x)[i]; }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return x; }
        get_y_type get_y() const { return y; }
        get_z_type get_z() const { return z; }
        get_w_type get_w() const { return w; }

        template<typename V> void set_x(const V &val) { x = val; }
        template<typename V> void set_y(const V &val) { y = val; }
        template<typename V> void set_z(const V &val) { z = val; }
        template<typename V> void set_w(const V &val) { w = val; }

        typedef dir3::packed imag_type;
        typedef f32          real_type;

        const imag_type &get_imag() const { return reinterpret_cast<const imag_type&>(*this); }
        const real_type &get_real() const { return w; }

        template<typename V> void set_real(const V &val) { w = val; }

        typedef quat_class<void, false, rep_std<true>> packed;
        typedef const quat_class<void, false, rep_constant> constant;

        template<typename A1, bool U1, typename R1>
        quat_class &operator=(const quat_class<A1, U1, R1> &a) { a.val().store4((f32*)this); return *this; }

        template<typename T> quat_class &operator+=(const T &a) { return *this = *this + a; }
        template<typename T> quat_class &operator-=(const T &a) { return *this = *this - a; }
        template<typename T> quat_class &operator*=(const T &a) { return *this = *this * a; }
        template<typename T> quat_class &operator/=(const T &a) { return *this = *this / a; }
    };

    template<>
    class quat_class<void, true> { // unit_quaternion

    protected:
        float4 v;

    public:
        explicit quat_class(const float4 &_v) : v(_v) {}

        const float4 &val() const { return v; }

        quat_class() {}

        template<typename X, typename Y, typename Z, typename W>
        quat_class(const X &x, const Y &y, const Z &z, const W &w) : v(x, y, z, w) {}

        template<typename X, typename Y, typename Z>
        quat_class(const X &x, const Y &y, const Z &z) : v(x, y, z, sqrt_abs(reinterpret_cast<const float1&>(float1_one) - abs_squared(dir3(x, y, z)))) {}

        template<typename A1, typename R1>
        quat_class(const quat_class<A1, true, R1> &a) : v(a.val()) {}

        template<typename A1, typename R1>
        explicit quat_class(const quat_class<A1, false, R1> &a) : v(unitize(a).val()) {}

        template<typename A1, typename U1, typename R1>
        quat_class(const vec_class<3, 0, A1, U1, R1> &a) : v(a.get_x(), a.get_y(), a.get_z(), sqrt_abs(reinterpret_cast<const float1&>(float1_one) - abs_squared(a))) {}

        template<typename A1, typename U1, typename R1, typename W>
        quat_class(const vec_class<3, 0, A1, U1, R1> &a, const W &w) : v(a.get_x(), a.get_y(), a.get_z(), w) {}

        template<typename T> const f32 &operator[](T i) const { return v[i]; }

        typedef float4::get_x_type get_x_type;
        typedef float4::get_y_type get_y_type;
        typedef float4::get_z_type get_z_type;
        typedef float4::get_w_type get_w_type;

        const get_x_type &get_x() const { return v.get_x(); }
        const get_y_type &get_y() const { return v.get_y(); }
        const get_z_type &get_z() const { return v.get_z(); }
        const get_w_type &get_w() const { return v.get_w(); }

        typedef dir3               imag_type;
        typedef float4::get_w_type real_type;

        const imag_type &get_imag() const { return reinterpret_cast<const imag_type&>(v); }
        const real_type &get_real() const { return v.get_w(); }

        typedef quat_class<void, true, rep_std<true>> packed;
        typedef const quat_class<void, true, rep_constant> constant;

        template<typename A1, typename R1>
        quat_class &operator=(const quat_class<A1, true, R1> &a) { v = a.val(); return *this; }

        template<typename A1, typename R1> quat_class &operator*=(const quat_class<A1, true, R1> &a) { return *this = *this * a; }
        template<typename A1, typename R1> quat_class &operator/=(const quat_class<A1, true, R1> &a) { return *this = *this / a; }
    };

    template<>
    class quat_class<void, true, rep_std<true>> { // unit_quaternion::packed

    protected:
        f32 x, y, z, w;

    public:
        explicit quat_class(const float4 &_v) { _v.store4((f32*)this); }

        float4 val() const {
            float4 result;
            result.load4((f32*)this);

            return result;
        }

        quat_class() {}

        template<typename X, typename Y, typename Z, typename W>
        quat_class(const X &_x, const Y &_y, const Z &_z, const W &_w) : x(_x), y(_y), z(_z), w(_w) {}

        template<typename X, typename Y, typename Z>
        quat_class(const X &_x, const Y &_y, const Z &_z) : x(_x), y(_y), z(_z), w(sqrt_abs(1.0f - abs_squared(dir3(_x, _y, _z)))) {}

        template<typename A1, typename R1>
        quat_class(const quat_class<A1, true, R1> &a) { a.val().store4((f32*)this); }

        template<typename A1, typename R1>
        explicit quat_class(const quat_class<A1, false, R1> &a) { unitize(a).val().store4((f32*)this); }

        template<typename A1, typename U1, typename R1>
        quat_class(const vec_class<3, 0, A1, U1, R1> &xyz) : w(sqrt_abs(1.0f - abs_squared(xyz))) { float4(xyz.val34()).store3((f32*)this); }

        template<typename A1, typename U1, typename R1, typename W>
        quat_class(const vec_class<3, 0, A1, U1, R1> &xyz, const W &_w) : w(_w) { float4(xyz.val34()).store3((f32*)this); }

        template<typename T> const f32 &operator[](T i) const { return (&x)[i]; }
        template<typename T>       f32 &operator[](T i)       { return (&x)[i]; }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return x; }
        get_y_type get_y() const { return y; }
        get_z_type get_z() const { return z; }
        get_w_type get_w() const { return w; }

        typedef dir3::packed imag_type;
        typedef f32          real_type;

        const imag_type &get_imag() const { return reinterpret_cast<const imag_type&>(*this); }
        const real_type &get_real() const { return w; }

        typedef quat_class<void, true, rep_std<true>> packed;
        typedef const quat_class<void, true, rep_constant> constant;

        template<typename A1, typename R1>
        quat_class &operator=(const quat_class<A1, true, R1> &a) { a.val().store4((f32*)this); return *this; }

        template<typename A1, typename R1> quat_class &operator*=(const quat_class<A1, true, R1> &a) { return *this = *this * a; }
        template<typename A1, typename R1> quat_class &operator/=(const quat_class<A1, true, R1> &a) { return *this = *this / a; }
    };

    template<bool U0>
    class quat_class<void, U0, rep_constant> {

    public:
        f32 x_, y_, z_, w_;

        operator const quat_class<void, U0>&() const { return reinterpret_cast<const quat_class<void, U0>&>(*this); }

        const float4 &val() const { return reinterpret_cast<const float4&>(*this); }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        get_x_type get_x() const { return x_; }
        get_y_type get_y() const { return y_; }
        get_z_type get_z() const { return z_; }
        get_w_type get_w() const { return w_; }
    };

    template<i32 n, typename A, bool U, typename R>
    typename get_set<n, quat_class<A, U, R>>::type get(const quat_class<A, U, R> &a) {
        return get_set<n, quat_class<A, U, R>>::get(a);
    }

    template<i32 n, typename A, bool U, typename R, typename V>
    void set(quat_class<A, U, R> &a, const V &v) {
        get_set<n, quat_class<A, U, R>>::set(a, v);
    }

    template<typename A, typename R>
    dir3 get_dir3(const quat_class<A, true, R> &a) {
        return (a.get_w() >= 0.0f) ? a.get_imag() : -a.get_imag();
    }

    template<typename A, bool U, typename R>
    quat_class<A, U> operator-(const quat_class<A, U, R> &a) {
        return quat_class<A, U>(-a.val());
    }

    template<typename A, bool U, typename R>
    quat_class<A, U> inv(const quat_class<A, U, R> &a) {
        return quat_class<A, U>((~a).val() / abs_squared(a));
    }

    template<typename A, bool U, typename R>
    quat_class<A, U> inv_safe(const quat_class<A, U, R> &a) {
        float1 aa = abs_squared(a);

        return aa ? quat_class<A, U>((~a).val() / aa) : quat_class<A, U>(a);
    }

    template<typename A, typename R>
    quat_class<A, true> inv(const quat_class<A, true, R> &a) {
        return ~a;
    }

    template<typename A, typename R>
    quat_class<A, true> inv_safe(const quat_class<A, true, R> &a) {
        return ~a;
    }

    inline const float4::constant float4_nnnp = { -1.0f, -1.0f, -1.0f, 1.0f };

    template<typename A, bool U, typename R>
    quat_class<A, U> operator~(const quat_class<A, U, R> &a) {
        return quat_class<A, U>(a.val() * reinterpret_cast<const float4&>(float4_nnnp));
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    float1 dot4(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return dot4(float4(a.val()), float4(b.val()));
    }

    template<typename A, bool U, typename R>
    float1 abs_squared(const quat_class<A, U, R> &a) {
        return dot4(a, a);
    }

    template<typename A, bool U, typename R>
    float1 abs(const quat_class<A, U, R> &a) {
        return sqrt(abs_squared(a));
    }

    template<typename A, bool U, typename R>
    quat_class<A, true> unitize(const quat_class<A, U, R> &a) {
        return quat_class<A, true>(a.val() * rsqrt(abs_squared(a)));
    }

    template<typename A, bool U, typename R>
    const quat_class<A, true, R> &declare_unit(const quat_class<A, U, R> &a) {
        return reinterpret_cast<const quat_class<A, true, R>&>(a);
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    quaternion operator+(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return quaternion(a.val() + b.val());
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    quaternion operator-(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return quaternion(a.val() - b.val());
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    bool operator==(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return a.val() == b.val();
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    bool operator!=(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return a.val() != b.val();
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator+(f32 a, const quat_class<A1, U1, R1> &b) {
        return quat_class<A1>(float4(float1_axis<3>(a)) + b.val());
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator+(const quat_class<A1, U1, R1> &a, f32 b) {
        return quat_class<A1>(a.val() + float4(float1_axis<3>(b)));
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator-(f32 a, const quat_class<A1, U1, R1> &b) {
        return quat_class<A1>(float4(float1_axis<3>(a)) - b.val());
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator-(const quat_class<A1, U1, R1> &a, f32 b) {
        return quat_class<A1>(a.val() - float4(float1_axis<3>(b)));
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    quat_class<void, (U1 && U2)> operator*(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return quat_class<void, (U1 && U2)>(mul_quat(a.val(), b.val()));
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator*(f32 a, const quat_class<A1, U1, R1> &b) {
        return quat_class<A1>(a * b.val());
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator*(const quat_class<A1, U1, R1> &a, f32 b) {
        return quat_class<A1>(a.val() * b);
    }

    template<typename A1, bool U1, typename R1,
             typename A2, bool U2, typename R2>
    quat_class<void, (U1 && U2)> operator/(const quat_class<A1, U1, R1> &a, const quat_class<A2, U2, R2> &b) {
        return quat_class<void, (U1 && U2)>(a * inv(b));
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator/(f32 a, const quat_class<A1, U1, R1> &b) {
        return quat_class<A1>(a * inv(b.val()));
    }

    template<typename A1, bool U1, typename R1>
    quat_class<A1> operator/(const quat_class<A1, U1, R1> &a, f32 b) {
        return quat_class<A1>(a.val() / b);
    }

    template<typename A1, bool U1, typename R1>
    float1 multiply_identity(const quat_class<A1, U1, R1>&) {
        return float1_one;
    }

    // sub_48D240
    template<typename A1, typename R1,
             typename A2, typename R2>
    unit_quaternion slerp(f32 weight_a, f32 weight_b, const quat_class<A1, true, R1> &a, const quat_class<A2, true, R2> &b) {
        const float4 qa = a.val();
        const float4 qb = b.val();

        f32 c        = (f32)((f64)qb.get_x() * qa.get_x() + (f64)qb.get_y() * qa.get_y() + (f64)qb.get_z() * qa.get_z() + (f64)qb.get_w() * qa.get_w());
        f32 scale[3] = { weight_a, weight_b, 1.0f };

        if (c < 0.0f) {
            scale[1] = -0.0f - weight_b;
            c        = -0.0f - c;
        }

        if (c < 0.999999f) {
            f32 angle;

            if (c < 0.5f) {
                f32 x2 = c  * c;
                f32 x3 = x2 * c;
                f32 x5 = x3 * x2;
                f32 x7 = x5 * x2;

                angle = -5.398124e-2f * x7;
                angle = (f32)((f64)angle - (f64)0.075f     * x5);
                angle = (f32)((f64)angle - (f64).1666667f * x3);
                angle -= c;
                angle += 1.570796f;
            } else {
                f32 s = sqrt((f32)((1.0 - (f64)c) * 0.5));

                f32 x2 = s  * s;
                f32 x3 = x2 * s;
                f32 x5 = x3 * x2;
                f32 x7 = x5 * x2;

                angle = .1079625f * x7;
                angle = (f32)((f64)angle + (f64).15f      * x5);
                angle = (f32)((f64)angle + (f64).3333333f * x3);
                angle = (f32)((f64)angle + 2.0             * s);
            }

            for (i32 lane = 0; lane < 3; lane++) {
                f32 x  = scale[lane] * angle;
                f32 x2 = x  * x;
                f32 x3 = x2 * x;
                f32 x5 = x2 * x3;
                f32 x7 = x2 * x5;

                f32 sine = (f32)((f64)x - (f64)1.917616e-4f * x7);
                sine = (f32)((f64)8.333334e-3f * x5 + sine);
                sine = (f32)((f64)sine - (f64).1666667f * x3);

                scale[lane] = sine;
            }

            scale[0] = scale[0] / scale[2];
            scale[1] = scale[1] / scale[2];
        }

        return unit_quaternion(float4((f32)((f64)(qa.get_x() * scale[0]) + (f64)qb.get_x() * scale[1]),
                                      (f32)((f64)(qa.get_y() * scale[0]) + (f64)qb.get_y() * scale[1]),
                                      (f32)((f64)(qa.get_z() * scale[0]) + (f64)qb.get_z() * scale[1]),
                                      (f32)((f64)(qa.get_w() * scale[0]) + (f64)qb.get_w() * scale[1])));
    }

    template<typename A1, typename R1,
             typename A2, typename R2>
    unit_quaternion slerp(f32 t, const quat_class<A1, true, R1> &a, const quat_class<A2, true, R2> &b) {
        return slerp(1.0f - t, t, a, b);
    }

    ASSERT_SIZEOF(quaternion,      0x10);
    ASSERT_SIZEOF(unit_quaternion, 0x10);
}} // treyarch::math
