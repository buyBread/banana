#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_camera_widget : public igo_3d_widget {
    public:
        u8        reserved_150[0x10];
        matrix4x4 camera_matrix;
        u8        matrix_dirty;
    };

    ASSERT_OFFSETOF(igo_3d_camera_widget, camera_matrix, 0x160);
    ASSERT_OFFSETOF(igo_3d_camera_widget, matrix_dirty,  0x1A0);
} // treyarch
