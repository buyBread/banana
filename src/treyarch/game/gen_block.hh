#pragma once

#include "treyarch/shared/mash/array_replacement.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/math/algebra.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class gb_road {

    public:
        u16   pos[2];
        u16   junction[2];
        u16   corner[2][2];
        u16   def;
        u16   retrofits_seed;
        u8    flags[2];
        u8    crosswalk[2];
        u8    do_depth_test;
        u8    neighborhood;
        u16   left;
        u16   right;
        u8    retrofit_category_bitmask;
        u8    pad_1f;
        u32   unk_20;
        void* unpacked;
        f32   last_cam_distance;
        u8    unk_2c[7];
        u8    unk_33;

        mash::array_replacement
            <mash::vector_basic<u16>, 2> traffic_lanes;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class gb_sidewalk_corner {

    public:
        u16   road[2];
        u16   pos;
        i8    dir[2];
        u16   unused;
        u8    do_depth_test;
        u8    flags;
        void* unpacked;
        f32   last_cam_distance;

        // inlined @ sub_7DEAC0
        void construct_mashed_class() {
            unpacked          = nullptr;
            last_cam_distance = 0.0f;
        }

        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    class gb_block {

    public:
        u8                      degree;
        u8                      flags;
        u8                      unused[2];
        mash::vector_basic<u16> vert;
        mash::vector_basic<u16> road;

        // sub_7D3B60
        ~gb_block() {
            finalize(mash::ALLOCATED);
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

    private:
        void finalize(mash::allocation_scope scope);
    };

    class gb_junction {

    public:
        u16                     pos;
        mash::vector_basic<u16> roads;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class gb_road_def {

    public:
        f32 median_width;
        f32 road_width;
        f32 sidewalk_width;
        u8  hole_along[2];
        u8  hole_across;
        u8  hole_length;
        u8  hole_width;
        u8  road_directions;
        i8  road_flip;
        u8  in_junction;
        u8  junction_type[2];
        u8  crosswalk_type[2];
        f32 crosswalk_width[2];
        u32 unk_20;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // the level's type-12 GenBlockMap resource, named after the level
    class gb_map {

    public:
        void*                     self; // the resource handler points this at the map itself
        u32                       version;
        u32                       memsize;
        mash::vector_basic
            <vector3d>            vertices;
        mash::vector
            <gb_road>             roads;
        mash::vector
            <gb_sidewalk_corner>  sidewalk_corners;
        mash::vector
            <gb_block>            blocks;
        mash::vector
            <gb_junction>         junctions;
        mash::vector
            <gb_road_def>         road_defs;
        vector3d                  last_cam_pos;
        resource_key              unk_8c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (gb_road,                0x38);
    ASSERT_OFFSETOF(gb_road, def,           0x10);
    ASSERT_OFFSETOF(gb_road, left,          0x1A);
    ASSERT_OFFSETOF(gb_road, unk_20,        0x20);
    ASSERT_OFFSETOF(gb_road, unpacked,      0x24);
    ASSERT_OFFSETOF(gb_road, unk_33,        0x33);
    ASSERT_OFFSETOF(gb_road, traffic_lanes, 0x34);

    ASSERT_SIZEOF  (gb_sidewalk_corner,                    0x14);
    ASSERT_OFFSETOF(gb_sidewalk_corner, unpacked,          0x0C);
    ASSERT_OFFSETOF(gb_sidewalk_corner, last_cam_distance, 0x10);

    ASSERT_SIZEOF  (gb_block,       0x24);
    ASSERT_OFFSETOF(gb_block, vert, 0x04);
    ASSERT_OFFSETOF(gb_block, road, 0x14);

    ASSERT_SIZEOF  (gb_junction,        0x14);
    ASSERT_OFFSETOF(gb_junction, roads, 0x04);

    ASSERT_SIZEOF  (gb_road_def,                  0x24);
    ASSERT_OFFSETOF(gb_road_def, crosswalk_width, 0x18);

    ASSERT_SIZEOF  (gb_map,                   0x94);
    ASSERT_OFFSETOF(gb_map, vertices,         0x0C);
    ASSERT_OFFSETOF(gb_map, roads,            0x1C);
    ASSERT_OFFSETOF(gb_map, sidewalk_corners, 0x30);
    ASSERT_OFFSETOF(gb_map, blocks,           0x44);
    ASSERT_OFFSETOF(gb_map, junctions,        0x58);
    ASSERT_OFFSETOF(gb_map, road_defs,        0x6C);
    ASSERT_OFFSETOF(gb_map, last_cam_pos,     0x80);
    ASSERT_OFFSETOF(gb_map, unk_8c,           0x8C);
} // treyarch
