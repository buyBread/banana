#pragma once

#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every spawn item; mashed ones get their retail vtable from the type registry
    class static_spawn_item : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x4C];
    };

    class static_spawn_item_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::vector
            <static_spawn_item> spawn_items;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (static_spawn_item, 0x50);

    ASSERT_SIZEOF  (static_spawn_item_collection,              0x18);
    ASSERT_OFFSETOF(static_spawn_item_collection, spawn_items, 0x04);
} // treyarch
