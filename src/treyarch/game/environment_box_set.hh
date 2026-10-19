#pragma once

#include "treyarch/game/static_colgeom_root.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class environment;
    class subdivision_node;
    struct region;

    // the set's activate resolves the environment and links the box to its node
    class environment_box {

    public:
        string_hash            environment_filename; // a type-30 environment
        string_hash            name;
        treyarch::environment* environment;
        subdivision_node*      node;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // each region's environment boxes, one per object in the root
    class environment_box_set {

    public:
        void* self; // the resource handler points this at the set itself

        region*             my_region;
        static_colgeom_root root;
        mash::vector
            <environment_box> environment_boxes;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (environment_box,              0x10);
    ASSERT_OFFSETOF(environment_box, name,        0x04);
    ASSERT_OFFSETOF(environment_box, environment, 0x08);
    ASSERT_OFFSETOF(environment_box, node,        0x0C);

    ASSERT_SIZEOF  (environment_box_set,                    0x4C);
    ASSERT_OFFSETOF(environment_box_set, my_region,         0x04);
    ASSERT_OFFSETOF(environment_box_set, root,              0x08);
    ASSERT_OFFSETOF(environment_box_set, environment_boxes, 0x38);
} // treyarch
