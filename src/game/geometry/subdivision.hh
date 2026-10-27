#pragma once

#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3's subdivision.h; every node starts with its type
    struct subdivision_node {
        enum e_type : u8 {
            undefined_node,
            proximity_map_node,
            static_leaf_list_node,
            dynamic_leaf_list_node,
            aabb_leaf_node,
            aabb_large_leaf_node,
            obb_leaf_node,
            obb_large_leaf_node,
            audio_obb_leaf_node,
            lego_leaf_node,
            static_lego_list_node,
            static_region_list_node,
            dynamic_entity_list_node,
            aabsp_node,
            building_leaf_node,
            total_types,
            visited_flag = 0x80
        };

        u8 type;
    };

    struct subdivision_node_obb_base : subdivision_node {
        u8          unk_01;
        u16         unk_02;
        u8          unk_04[0x0C];
        string_hash unk_10;
        vector3d    center;
        u32         unk_20;
        u16         unk_24;
    };

    // the axes are unit length; their lengths are the half extents
    struct subdivision_node_large_obb : subdivision_node_obb_base {
        vector3d x_axis;
        vector3d y_axis;
        vector3d z_axis;
        f32      x_length;
        f32      y_length;
        f32      z_length;
    };

    ASSERT_SIZEOF  (subdivision_node_obb_base,         0x28);
    ASSERT_OFFSETOF(subdivision_node_obb_base, unk_10, 0x10);
    ASSERT_OFFSETOF(subdivision_node_obb_base, center, 0x14);
    ASSERT_OFFSETOF(subdivision_node_obb_base, unk_24, 0x24);

    ASSERT_SIZEOF  (subdivision_node_large_obb,           0x58);
    ASSERT_OFFSETOF(subdivision_node_large_obb, x_axis,   0x28);
    ASSERT_OFFSETOF(subdivision_node_large_obb, x_length, 0x4C);
} // treyarch
