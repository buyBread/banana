#pragma once

#include "treyarch/shared/math/vmath/matrix_rot.hh"
#include "treyarch/shared/math/vmath/trig.hh"

namespace treyarch { namespace math {
    inline rot_mat33 axis_sin_cos_to_rot_mat(const unit_dir3 &axis, const float1 &s, const float1 &c) {
        float4 r0, r1, r2;
        axis_sin_cos_to_matrix(axis.val34(), s, c, r0, r1, r2);

        return rot_mat33(unit_dir3(r0), unit_dir3(r1), unit_dir3(r2));
    }

    inline rot_mat33 axis_angle_to_rot_mat(const unit_dir3 &axis, const float1 &angle) {
        const float4 &sc = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_any, sin_cos_any>(angle);

        float4 r0, r1, r2;
        axis_sin_cos_to_matrix(axis.val34(), sc.get_x(), sc.get_y(), r0, r1, r2);

        return rot_mat33(unit_dir3(r0), unit_dir3(r1), unit_dir3(r2));
    }

    inline rot_mat33 axis_angle_to_rot_mat(const unit_dir_x&, const float1 &angle) {
        float4 scnz = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_negsin, sin_cos_negcos>(angle);
        scnz.set_w(float4_zeroes.x);

        return rot_mat33(unit_dir_x(), unit_dir3(swizzle<3, 1, 0, 3>(scnz)), unit_dir3(swizzle<3, 2, 1, 3>(scnz)));
    }

    inline rot_mat33 axis_angle_to_rot_mat(const unit_dir_y&, const float1 &angle) {
        float4 scnz = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_negsin, sin_cos_negcos>(angle);
        scnz.set_w(float4_zeroes.x);

        return rot_mat33(unit_dir3(swizzle<1, 3, 2, 3>(scnz)), unit_dir_y(), unit_dir3(swizzle<0, 3, 1, 3>(scnz)));
    }

    inline rot_mat33 axis_angle_to_rot_mat(const unit_dir_z&, const float1 &angle) {
        float4 scnz = sin_cos<sin_cos_sin, sin_cos_cos, sin_cos_negsin, sin_cos_negcos>(angle);
        scnz.set_w(float4_zeroes.x);

        return rot_mat33(unit_dir3(swizzle<1, 0, 3, 3>(scnz)), unit_dir3(swizzle<2, 1, 3, 3>(scnz)), unit_dir_z());
    }

    inline rot_mat33 axis180_to_rot_mat(const unit_dir3 &axis) {
        float4 r0, r1, r2;
        axis180_to_matrix(axis.val34(), r0, r1, r2);

        return rot_mat33(unit_dir3(r0), unit_dir3(r1), unit_dir3(r2));
    }

    inline const unit_dir3::constant to_axis_sin_cos_arbitrary = { 1.0f, 0.0f, 0.0f };

    inline void rot_mat_to_axis_sin_cos(const rot_mat33 &rot, unit_dir3 &axis, f32 &s, f32 &c) {
        const float4 &r0 = rot.get_x().val34();
        const float4 &r1 = rot.get_y().val34();
        const float4 &r2 = rot.get_z().val34();

        c = 0.5f * (r0.get_x() + r1.get_y() + r2.get_z() - 1.0f);
        s = sqrt_abs(1.0f - c * c);

        if (c < -0.99995f)
            axis = unit_dir3(sqrt_abs((float4(r0.get_x(), r1.get_y(), r2.get_z()) - c) / (1.0f - c)));
        else if (s >= 0.01f)
            axis = unit_dir3(float4(r1.get_z() - r2.get_y(), r2.get_x() - r0.get_z(), r0.get_y() - r1.get_x()) * (0.5f / s));
        else if (s > 1e-8f)
            axis = unitize(dir3(r1.get_z() - r2.get_y(), r2.get_x() - r0.get_z(), r0.get_y() - r1.get_x()));
        else
            axis = to_axis_sin_cos_arbitrary;
    }

    inline void rot_mat_to_axis_angle(const rot_mat33 &rot, unit_dir3 &axis, f32 &angle) {
        f32 s, c;
        rot_mat_to_axis_sin_cos(rot, axis, s, c);

        angle = atan_upper(s, c);
    }

    inline const float4::constant axis_angle_to_quaternion_phase = { math_pi, math_pi, math_pi, 0.0f };
    inline const float4::constant axis_angle_to_quaternion_freq  = { 0.07957747f, 0.07957747f, 0.07957747f, 0.07957747f };

    // sub_9CCB20
    inline unit_quaternion axis_angle_to_quaternion(const unit_dir3 &dir, const float1 &angle) {
        const float4 &ns_ns_ns_c = cos(angle + axis_angle_to_quaternion_phase, axis_angle_to_quaternion_freq);

        return unit_quaternion(dir3(dir.val34() * ns_ns_ns_c), ns_ns_ns_c.get_w());
    }

    inline void quaternion_to_axis_sin_cos(const unit_quaternion &q, unit_dir3 &axis, f32 &s, f32 &c) {
        f32    ch = q.get_w();
        float1 sh = abs(q.get_imag());

        if (sh)
            axis = declare_unit(q.get_imag() / sh);
        else
            axis = to_axis_sin_cos_arbitrary;

        s = (ch + ch) * sh;
        c = ch * ch - sh * sh;
    }

    inline void quaternion_to_axis_angle(const unit_quaternion &q, unit_dir3 &axis, f32 &angle) {
        f32    ch = q.get_w();
        float1 sh = abs(q.get_imag());

        if (sh)
            axis = declare_unit(q.get_imag() / sh);

        angle = 2.0f * atan_upper(sh, ch);
    }
}} // treyarch::math
