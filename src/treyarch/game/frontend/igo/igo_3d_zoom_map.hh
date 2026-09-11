#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_zoom_map {

    public:
        u8 reserved_000[0x0D];
        u8 render_block_00d;
        u8 reserved_00e[0x8F82];
        u8 render_block_8f90;

        bool blocks_world_rendering() const {
            return render_block_00d || render_block_8f90;
        }
    };

    ASSERT_OFFSETOF(igo_3d_zoom_map, render_block_00d,  0x000D);
    ASSERT_OFFSETOF(igo_3d_zoom_map, render_block_8f90, 0x8F90);
} // treyarch
