#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // one of the zoom map's poi records
    struct zoom_map_poi {
        u8       reserved_00[0x04];
        vector3d position;
        u8       reserved_10[0x10];
        bool     active;
        u8       reserved_21[0x2F];
    };

    class igo_3d_zoom_map {

    public:
        igo_3d_drawable_vtable* vtable;

        u8           reserved_004[0x09];
        u8           render_block_00d;
        u8           reserved_00e[0xF2];
        zoom_map_poi pois[0x1C2];
        u8           reserved_8da0[0x1F0];
        u8           render_block_8f90;
        u8           reserved_8f91[0x04];
        u8           unk_8f95;          // the cut scene player sets both while it has the igo
        u8           unk_8f96;

        bool blocks_world_rendering() const {
            return render_block_00d || render_block_8f90;
        }

        void draw_map_overlay() {
            vtable->method_06c((igo_3d_drawable*)this);
        }
    };

    ASSERT_SIZEOF  (zoom_map_poi,           0x50);
    ASSERT_OFFSETOF(zoom_map_poi, position, 0x04);
    ASSERT_OFFSETOF(zoom_map_poi, active,   0x20);

    ASSERT_OFFSETOF(igo_3d_zoom_map, render_block_00d,  0x000D);
    ASSERT_OFFSETOF(igo_3d_zoom_map, pois,              0x0100);
    ASSERT_OFFSETOF(igo_3d_zoom_map, render_block_8f90, 0x8F90);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_8f95,          0x8F95);
    ASSERT_OFFSETOF(igo_3d_zoom_map, unk_8f96,          0x8F96);
} // treyarch
