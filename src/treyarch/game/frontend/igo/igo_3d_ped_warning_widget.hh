#pragma once

#include "treyarch/ngl/quad/quad.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_ped_warning_widget {

    public:
        void*      vtable;
        ngl::quad* quad;
        bool       visible;
        u8         reserved_009[0x1B];
        f32        alpha;

        void draw();
    };

    ASSERT_OFFSETOF(igo_3d_ped_warning_widget, quad,    0x04);
    ASSERT_OFFSETOF(igo_3d_ped_warning_widget, visible, 0x08);
    ASSERT_OFFSETOF(igo_3d_ped_warning_widget, alpha,   0x24);
} // treyarch
