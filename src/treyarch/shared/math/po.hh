#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"

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
    };

    ASSERT_SIZEOF(po, 0x40);
} // treyarch
