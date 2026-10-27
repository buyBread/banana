#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct rpg_upgrade {
        mash::string            name;
        mash::string            title;
        mash::string            description;
        mash::string            instructions;
        mash::string            unk_30;
        mash::string            unk_3c;
        i32                     unk_48; // 3 puts it on the upgrade manager's second list
        i32                     unk_4c;
        f32                     cost;
        i32                     unk_54;
        i32                     unk_58;
        mash::vector_basic<i32> unk_5c; // purchases are refused once any of these upgrades is owned
        mash::string            quest;
        u32                     unk_78[10];
        i32                     unk_a0; // 3 once purchased
        i32                     unk_a4;
        i32                     unk_a8;
        bool                    unk_ac;
        u8                      pad_ad[3];

        // inlined @ sub_93CC10
        void construct_mashed_class() {
            name        .construct_mashed_class();
            title       .construct_mashed_class();
            description .construct_mashed_class();
            instructions.construct_mashed_class();
            unk_30      .construct_mashed_class();
            unk_3c      .construct_mashed_class();
            unk_5c      .construct_mashed_class();
            quest       .construct_mashed_class();
        }

        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class rpg_upgrades {

    public:
        void* self; // the resource handler points this at the upgrades themselves

        mash::vector<rpg_upgrade> upgrades;
        mash::vector_basic<i32>   unk_18;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (rpg_upgrade,         0xB0);
    ASSERT_OFFSETOF(rpg_upgrade, unk_48, 0x48);
    ASSERT_OFFSETOF(rpg_upgrade, cost,   0x50);
    ASSERT_OFFSETOF(rpg_upgrade, unk_5c, 0x5C);
    ASSERT_OFFSETOF(rpg_upgrade, quest,  0x6C);
    ASSERT_OFFSETOF(rpg_upgrade, unk_78, 0x78);
    ASSERT_OFFSETOF(rpg_upgrade, unk_a0, 0xA0);
    ASSERT_OFFSETOF(rpg_upgrade, unk_ac, 0xAC);

    ASSERT_SIZEOF  (rpg_upgrades,           0x28);
    ASSERT_OFFSETOF(rpg_upgrades, upgrades, 0x04);
    ASSERT_OFFSETOF(rpg_upgrades, unk_18,   0x18);
} // treyarch
