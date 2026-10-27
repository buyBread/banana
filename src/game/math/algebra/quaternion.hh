#pragma once

#include "game/math/algebra/matrix4x4.hh"

namespace treyarch {
    typedef math::quat_class<void, true, math::rep_sm2> quaternion;
} // treyarch

namespace treyarch { namespace math {
    template<>
    class quat_class<void, true, rep_sm2> { // quaternion, w first

    public:
        f32 a;
        f32 b;
        f32 c;
        f32 d;

        explicit quat_class(const float4 &_v) {
            swizzle<3, 0, 1, 2>(_v).store4((f32*)this);
        }

        float4 val() const {
            float4 result;
            result.load4((f32*)this);

            return swizzle<1, 2, 3, 0>(result);
        }

        typedef f32 get_x_type;
        typedef f32 get_y_type;
        typedef f32 get_z_type;
        typedef f32 get_w_type;

        const get_x_type &get_x() const { return d; }
        const get_y_type &get_y() const { return a; }
        const get_z_type &get_z() const { return b; }
        const get_w_type &get_w() const { return c; }

        quat_class() {
            a = 1.0f;
            b = c = d = 0.0f;
        }

        template<typename A1, typename R1>
        quat_class(const quat_class<A1, true, R1> &v) {
            swizzle<3, 0, 1, 2>(v.val()).store4((f32*)this);
        }

        template<typename A1, typename U1, typename R1>
        quat_class(const vec_class<3, 0, A1, U1, R1> &v) : a(sqrt_abs(reinterpret_cast<const float1&>(float1_one) - abs_squared(v))) {
            float4(v.val34()).store3((f32*)&b);
        }

        template<typename A1, typename U1, typename R1, typename W>
        quat_class(const vec_class<3, 0, A1, U1, R1> &v, const W &w) : a(w) {
            float4(v.val34()).store3((f32*)&b);
        }

        quat_class(f32 _a, f32 _b, f32 _c, f32 _d) : a(_a), b(_b), c(_c), d(_d) {}

        quat_class(const matrix4x4 &m) {
            unit_quaternion tmp = get_quaternion(reinterpret_cast<const rot_mat33&>(m));

            swizzle<3, 0, 1, 2>(tmp.val()).store4((f32*)this);
        }

        unit_quaternion get_conjugate() const { return ~*this; }

        void to_matrix(matrix4x4* dest) const {
            *dest = quat_mat33(*this);
        }

        typedef dir3::packed imag_type;
        typedef f32          real_type;

        const imag_type &get_imag() const { return reinterpret_cast<const dir3::packed&>(b); }
        const real_type &get_real() const { return a; }

        typedef       quat_class<void, true, rep_std<true>> packed;
        typedef const quat_class<void, true, rep_constant>  constant;

        template<typename A1, typename R1>
        quat_class &operator=(const quat_class<A1, true, R1> &q) {
            swizzle<3, 0, 1, 2>(q.val()).store4((f32*)this);
            
            return *this;
        }

        template<typename A1, typename R1> quat_class &operator*=(const quat_class<A1, true, R1> &q) { return *this = *this * q; }
        template<typename A1, typename R1> quat_class &operator/=(const quat_class<A1, true, R1> &q) { return *this = *this / q; }
    };

    inline treyarch::quaternion slerp(const treyarch::quaternion &q0,
                                      const treyarch::quaternion &q1,
                                            f32                   t) {

        return slerp(t, q0, q1);
    }
}} // treyarch::math

namespace treyarch {
    ASSERT_SIZEOF(quaternion, 0x10);
} // treyarch
