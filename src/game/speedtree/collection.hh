#pragma once

#include "game/speedtree/wrapper.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct speedtree_collection_entry {
        u32 unk_00;
        u32 unk_04;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // the level's type-7 resource, named after the level
    class speedtree_collection {

    public:
        void* self; // the resource handler points this at the collection itself
        
        mash::vector
            <speedtree_collection_entry> unk_04;
        mash::vector
            <speed_tree_color_source>    placements;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class speedtree_collection_reference {

    public:
        void* self; // the resource handler points this at the reference itself
        u32   unk_04[4];

        // inlined @ sub_764C30
        void construct_mashed_class() {
            self = nullptr;
        }

        // inlined @ sub_766FB0
        void destruct_mashed_class() {}

        // inlined @ sub_764C30
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    ASSERT_SIZEOF  (speedtree_collection_entry, 0x08);

    ASSERT_SIZEOF  (speedtree_collection,             0x2C);
    ASSERT_OFFSETOF(speedtree_collection, unk_04,     0x04);
    ASSERT_OFFSETOF(speedtree_collection, placements, 0x18);

    ASSERT_SIZEOF  (speedtree_collection_reference,         0x14);
    ASSERT_OFFSETOF(speedtree_collection_reference, unk_04, 0x04);
} // treyarch
