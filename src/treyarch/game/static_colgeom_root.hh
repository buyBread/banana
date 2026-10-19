#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct static_colgeom_tree_root {
        u8  unk_00[0x30];
        u8* unk_30; // an offset into the data block until unmashed
        u8* unk_34;
        u8* unk_38; // the data block
        u32 unk_3c;
    };

    struct static_colgeom_hull {
        u8  unk_00[0x14];
        u8* hull_mesh; // an offset into the convex hull mesh file until unmashed
        u8  unk_18[0x08];
    };

    // each region's static collision; the tree lives in one raw data block
    class static_colgeom_root {

    public:
        void* self; // the resource handler points this at the root itself

        u8*                       unk_04;      // an offset into data until unmashed
        static_colgeom_tree_root* tree_root;
        void*                     my_aabb;     // 0x38 bytes
        u32                       type;        // 3 for an environment box set's root
        u32                       num_objects; // no data block without any
        u8*                       data;
        u32                       data_size;
        string_hash               hull_meshes_file;
        u32                       hulls_offset;
        u32                       hull_count;
        u32                       unk_2c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (static_colgeom_tree_root,         0x40);
    ASSERT_OFFSETOF(static_colgeom_tree_root, unk_30, 0x30);

    ASSERT_SIZEOF  (static_colgeom_hull,            0x20);
    ASSERT_OFFSETOF(static_colgeom_hull, hull_mesh, 0x14);

    ASSERT_SIZEOF  (static_colgeom_root,                   0x30);
    ASSERT_OFFSETOF(static_colgeom_root, tree_root,        0x08);
    ASSERT_OFFSETOF(static_colgeom_root, num_objects,      0x14);
    ASSERT_OFFSETOF(static_colgeom_root, data,             0x18);
    ASSERT_OFFSETOF(static_colgeom_root, hull_meshes_file, 0x20);
    ASSERT_OFFSETOF(static_colgeom_root, hull_count,       0x28);
} // treyarch
