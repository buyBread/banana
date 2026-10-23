#pragma once

#include "vc8/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "treyarch/shared/math/algebra.hh"
#include "treyarch/shared/math/bound.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct region;

    class ai_path_id {

    public:
        string_hash region_id; // the region's name
        u32         path_id;
    };

    class ai_quad_path_connection {

    public:
        ai_path_id remote_path_id;
        u16        local_cell_index;
        u16        local_edge_index;
        u8         flags;
        u8         unk_0d;
        u8         unk_0e; // the graph collects the connections with 0x04 set when it's activated
        u8         unk_0f;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // a triangle or quad over its path's vertices
    class ai_quad_path_cell {

    public:
        u16 vertex_indices[4];
        i16 unk_08[4];
        u32 unk_10;
        u32 unk_14;
        u8  unk_18;
        u8  normal_code;
        u16 flags; // the cell type in the low four bits, ped spawnable at 0x10
        u32 unk_1c;
        u32 unk_20;
        u32 unk_24;
        u32 unk_28;

        ~ai_quad_path_cell();

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    class ai_quad_path {

    public:
        bounding_box                     path_bbox;
        region*                          my_region;
        ai_path_id                       my_path_id;
        u32                              unk_24; // 1234 in the image, 4321 once activated
        mash::vector
            <ai_quad_path_connection>    connection_table;
        mash::vector_basic<vector3d>     vertices;
        mash::vector<ai_quad_path_cell>  cell_table;
        vc8::vector<void*>*       obstacles; // the navmesh obstacle manager's
        u32                              unk_64;
        u32                              unk_68;
        mash::vector_basic<u16>          unk_6c;
        mash::vector_basic<u16>          unk_7c;

        // sub_442420
        ~ai_quad_path() {
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

    // the base of every region point; mashed ones get their retail vtable from the type registry
    class base_region_point : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x10];
    };

    // each region's navmeshes for pathfinding, keyed by region
    class alignas(8) ai_pf_region_graph {

    public:
        void* self; // the resource handler points this at the graph itself

        u32                             unk_04;
        engine_recursive_lock           lock;
        u32                             unk_18;
        region*                         my_region;
        string_hash                     my_district_id; // the region's name
        bounding_box                    region_path_bbox;
        mash::vector<ai_quad_path>      quad_path_table;
        mash::vector
            <base_region_point>         region_points;
        vc8::vector
            <ai_quad_path_connection*>* unk_64; // filled when the paths are activated

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (ai_path_id,          0x08);

    ASSERT_SIZEOF  (ai_quad_path_connection,                   0x10);
    ASSERT_OFFSETOF(ai_quad_path_connection, local_cell_index, 0x08);
    ASSERT_OFFSETOF(ai_quad_path_connection, flags,            0x0C);

    ASSERT_SIZEOF  (ai_quad_path_cell,              0x2C);
    ASSERT_OFFSETOF(ai_quad_path_cell, unk_10,      0x10);
    ASSERT_OFFSETOF(ai_quad_path_cell, normal_code, 0x19);
    ASSERT_OFFSETOF(ai_quad_path_cell, flags,       0x1A);
    ASSERT_OFFSETOF(ai_quad_path_cell, unk_24,      0x24);

    ASSERT_SIZEOF  (ai_quad_path,                   0x8C);
    ASSERT_OFFSETOF(ai_quad_path, my_region,        0x18);
    ASSERT_OFFSETOF(ai_quad_path, my_path_id,       0x1C);
    ASSERT_OFFSETOF(ai_quad_path, connection_table, 0x28);
    ASSERT_OFFSETOF(ai_quad_path, vertices,         0x3C);
    ASSERT_OFFSETOF(ai_quad_path, cell_table,       0x4C);
    ASSERT_OFFSETOF(ai_quad_path, obstacles,        0x60);
    ASSERT_OFFSETOF(ai_quad_path, unk_6c,           0x6C);
    ASSERT_OFFSETOF(ai_quad_path, unk_7c,           0x7C);

    ASSERT_SIZEOF  (base_region_point, 0x14);

    ASSERT_SIZEOF  (ai_pf_region_graph,                   0x68);
    ASSERT_OFFSETOF(ai_pf_region_graph, lock,             0x08);
    ASSERT_OFFSETOF(ai_pf_region_graph, my_region,        0x1C);
    ASSERT_OFFSETOF(ai_pf_region_graph, my_district_id,   0x20);
    ASSERT_OFFSETOF(ai_pf_region_graph, region_path_bbox, 0x24);
    ASSERT_OFFSETOF(ai_pf_region_graph, quad_path_table,  0x3C);
    ASSERT_OFFSETOF(ai_pf_region_graph, region_points,    0x50);
    ASSERT_OFFSETOF(ai_pf_region_graph, unk_64,           0x64);
} // treyarch
