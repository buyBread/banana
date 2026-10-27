#pragma once

#include "game/quest_descriptions.hh"
#include "game/reference.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct quest_unlock {
        mash::string     expression;
        mash::string     quest;
        mash::string     unk_18;
        quest_condition* condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // the base of every quest giver location; mashed ones get their retail vtable from the type registry
    class quest_giver_location_item : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x20];
    };

    struct quest_giver {
        mash::vector
            <quest_giver_location_item> locations;

        mash::string name;
        mash::string unk_20;
        u32          unk_2c;
        mash::string unk_30;
        mash::string unk_3c;
        mash::string unk_48;
        i32          unk_54;
        mash::string unk_58;
        mash::string greeting;
        mash::string unk_70;
        mash::string unk_7c;
        u32          unk_88[3];
        u32          unk_94;
        u32          unk_98;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct quest_list_unk_50 {
        mash::string unk_00;
        u32          unk_0c[6];

        // inlined @ sub_802410
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
        }

        // inlined @ sub_6CC4A0
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
        }

        // inlined @ sub_7FBB30
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_00.unmash(mash_info, this, buffer);
        }
    };

    class quest_list {

    public:
        void* self; // the resource handler points this at the list itself

        mash::vector
            <quest_unlock>                  unlocks;
        mash::vector
            <reference<quest_descriptions>> descriptions;
        mash::vector
            <quest_giver>                   givers;
        mash::vector_basic
            <vector3d>                      unk_40;
        mash::vector
            <quest_list_unk_50>             unk_50;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (quest_unlock,            0x28);
    ASSERT_OFFSETOF(quest_unlock, condition, 0x24);

    ASSERT_SIZEOF  (quest_giver_location_item, 0x24);

    ASSERT_SIZEOF  (quest_giver,           0x9C);
    ASSERT_OFFSETOF(quest_giver, name,     0x14);
    ASSERT_OFFSETOF(quest_giver, unk_2c,   0x2C);
    ASSERT_OFFSETOF(quest_giver, unk_54,   0x54);
    ASSERT_OFFSETOF(quest_giver, greeting, 0x64);
    ASSERT_OFFSETOF(quest_giver, unk_88,   0x88);
    ASSERT_OFFSETOF(quest_giver, unk_94,   0x94);

    ASSERT_SIZEOF  (quest_list_unk_50, 0x24);

    ASSERT_SIZEOF  (quest_list,               0x64);
    ASSERT_OFFSETOF(quest_list, descriptions, 0x18);
    ASSERT_OFFSETOF(quest_list, givers,       0x2C);
    ASSERT_OFFSETOF(quest_list, unk_40,       0x40);
    ASSERT_OFFSETOF(quest_list, unk_50,       0x50);
} // treyarch
