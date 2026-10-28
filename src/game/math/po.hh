#pragma once

#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch {
    class po {

    private:
        matrix4x4 m;

    public:
              matrix4x4 &to_mat()       { return m; }
        const matrix4x4 &to_mat() const { return m; }

              matrix4x4 &get_matrix()       { return m; }
        const matrix4x4 &get_matrix() const { return m; }

        const vector3d &get_position() const { return m.wrow(); }

        const vector3d &get_x_facing() const { return m.xrow(); }
        const vector3d &get_y_facing() const { return m.yrow(); }
        const vector3d &get_z_facing() const { return m.zrow(); }

        // inlined @ sub_863C30
        // inlined @ sub_865420
        vector3d slow_xform(const vector3d &point) const { // a point out of this po's frame
            return vector3d((f32)((f64)point.x * m.x.x + (f64)point.y * m.y.x + (f64)point.z * m.z.x + m.w.x),
                            (f32)((f64)point.x * m.x.y + (f64)point.y * m.y.y + (f64)point.z * m.z.y + m.w.y),
                            (f32)((f64)point.x * m.x.z + (f64)point.y * m.y.z + (f64)point.z * m.z.z + m.w.z));
        }

        // inlined @ sub_844CD0
        // inlined @ sub_846EE0
        vector3d non_affine_slow_xform(const vector3d &v) const { // a direction out of this po's frame
            return vector3d((f32)((f64)m.y.x * v.y + (f64)v.x * m.x.x + (f64)m.z.x * v.z + (f64)m.w.x * 0.0),
                            (f32)((f64)m.y.y * v.y + (f64)m.x.y * v.x + (f64)m.z.y * v.z + (f64)m.w.y * 0.0),
                            (f32)((f64)m.y.z * v.y + (f64)m.x.z * v.x + (f64)m.z.z * v.z + (f64)m.w.z * 0.0));
        }

        // inlined @ sub_8637C0
        // inlined @ sub_8639F0
        vector3d inverse_xform(const vector3d &point) const { // a point into this po's frame, the rotation taken as orthonormal
            const vector3d &t = m.wrow();

            const f32 x = -0.0f - (f32)((f64)m.x.x * t.x + (f64)m.x.y * t.y + (f64)m.x.z * t.z);
            const f32 y = -0.0f - (f32)((f64)m.y.x * t.x + (f64)m.y.y * t.y + (f64)m.y.z * t.z);
            const f32 z = -0.0f - (f32)((f64)m.z.x * t.x + (f64)m.z.y * t.y + (f64)m.z.z * t.z);

            return vector3d((f32)((f64)m.x.x * point.x + (f64)m.x.y * point.y + (f64)m.x.z * point.z + x),
                            (f32)((f64)m.y.x * point.x + (f64)m.y.y * point.y + (f64)m.y.z * point.z + y),
                            (f32)((f64)m.z.x * point.x + (f64)m.z.y * point.y + (f64)m.z.z * point.z + z));
        }
    };

    ASSERT_SIZEOF(po, 0x40);
} // treyarch
