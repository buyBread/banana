#pragma once

#include "game/progression_change.hh"
#include "game/quest_descriptions.hh"
#include "game/spawn_table_interface.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every no-spawn area; mashed ones get their retail vtable from the type registry
    class no_spawn_base : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x0C];
    };

    // the spawn table manager keeps the last one whose condition holds
    struct spawn_item_collection_unk_38 {
        mash::string     name;
        mash::string     expression;
        i32              unk_18;
        i32              unk_1c;
        f32              unk_20;
        f32              unk_24;
        f32              unk_28;
        f32              unk_2c;
        i32              unk_30;
        quest_condition* condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the spawn table manager keeps the last one whose condition holds
    struct spawn_item_collection_unk_4c {
        mash::string        name;
        mash::string        expression;
        mash::vector
            <no_spawn_base> no_spawn_areas;
        quest_condition*    condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class spawn_item_collection {

    public:
        void* self; // the resource handler points this at the collection itself

        mash::string                       name;
        mash::vector
            <progression_change>           changes;
        mash::vector
            <spawn_table_unk_68>           unk_24;
        mash::vector
            <spawn_item_collection_unk_38> unk_38;
        mash::vector
            <spawn_item_collection_unk_4c> unk_4c;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (no_spawn_base, 0x10);

    ASSERT_SIZEOF  (spawn_item_collection_unk_38,            0x38);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, unk_18,    0x18);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, unk_30,    0x30);
    ASSERT_OFFSETOF(spawn_item_collection_unk_38, condition, 0x34);

    ASSERT_SIZEOF  (spawn_item_collection_unk_4c,                 0x30);
    ASSERT_OFFSETOF(spawn_item_collection_unk_4c, no_spawn_areas, 0x18);
    ASSERT_OFFSETOF(spawn_item_collection_unk_4c, condition,      0x2C);

    ASSERT_SIZEOF  (spawn_item_collection,          0x60);
    ASSERT_OFFSETOF(spawn_item_collection, name,    0x04);
    ASSERT_OFFSETOF(spawn_item_collection, changes, 0x10);
    ASSERT_OFFSETOF(spawn_item_collection, unk_24,  0x24);
    ASSERT_OFFSETOF(spawn_item_collection, unk_38,  0x38);
    ASSERT_OFFSETOF(spawn_item_collection, unk_4c,  0x4C);
} // treyarch
