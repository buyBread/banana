#pragma once

#include "game/reference.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class lego;
    class proximity_map;

    struct lego_map_unk_40 {
        u16 unk_00;
        u16 unk_02;
    };

    // each region's legos: its static mesh instances
    struct lego_map_root_node {
        void* self; // the resource handler points this at the root itself

        u32                      unk_04;
        proximity_map*           grid;
        u32                      grid_size;
        u32                      num_regular_legos;  // 0x20 bytes each
        u32                      num_building_legos; // 0x90 bytes each, after the regular ones
        lego*                    legos;
        mash::vector
            <reference<tilemap>> tilemaps;
        mash::vector_basic
            <tl_fixed_string>    lego_mesh_names;
        mash::vector_basic
            <lego_map_unk_40>    unk_40;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (lego_map_unk_40, 0x04);

    ASSERT_SIZEOF  (lego_map_root_node,                  0x50);
    ASSERT_OFFSETOF(lego_map_root_node, grid,            0x08);
    ASSERT_OFFSETOF(lego_map_root_node, legos,           0x18);
    ASSERT_OFFSETOF(lego_map_root_node, tilemaps,        0x1C);
    ASSERT_OFFSETOF(lego_map_root_node, lego_mesh_names, 0x30);
    ASSERT_OFFSETOF(lego_map_root_node, unk_40,          0x40);
} // treyarch
