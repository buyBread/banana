#pragma once

#include "vc8/map.hh"
#include "treyarch/game/progression_change.hh"
#include "treyarch/game/quest_descriptions.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct summon_level {
        mash::string     unk_00;
        mash::string     unk_0c;
        quest_condition* condition;

        // inlined @ sub_93EFC0
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
            unk_0c.construct_mashed_class();

            if (condition)
                mash::mash_info_struct::construct_class(condition);
        }

        // inlined @ sub_928EF0
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
            unk_0c.destruct_mashed_class();

            if (condition) {
                condition->destruct_mashed_class();
                condition = nullptr;
            }
        }

        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct summon {
        mash::string       name;
        mash::string       unk_0c;
        f32                unk_18;
        i32                character; // an index into the ui character info list
        mash::vector
            <summon_level> levels;
        u32                unk_34;
        i32                unk_38;
        u32                unk_3c;
        f32                unk_40;
        f32                unk_44;
        f32                unk_48;
        f32                unk_4c;
        i32                level; // the last of levels whose condition holds
        f32                unk_54;

        // inlined @ sub_9540E0
        void construct_mashed_class() {
            name  .construct_mashed_class();
            unk_0c.construct_mashed_class();
            levels.construct_mashed_class();

            unk_54 = unk_18;
            level  = 0;
        }

        // inlined @ sub_93F020
        void destruct_mashed_class() {
            name  .destruct_mashed_class();
            unk_0c.destruct_mashed_class();
            levels.destruct_mashed_class();
        }

        // inlined @ sub_94D1F0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name  .unmash(mash_info, this, buffer);
            unk_0c.unmash(mash_info, this, buffer);
            levels.unmash(mash_info, this, buffer);
        }
    };

    class summon_progression {

    public:
        void* self; // the resource handler points this at the progression itself

        mash::vector
            <progression_change>     changes; // switches summons on and off
        mash::vector
            <summon>                 summons;
        vc8::map
            <mash::string, summon*>* summons_by_name; // built once the summon manager loads it

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (summon_level,            0x1C);
    ASSERT_OFFSETOF(summon_level, condition, 0x18);

    ASSERT_SIZEOF  (summon,            0x58);
    ASSERT_OFFSETOF(summon, unk_18,    0x18);
    ASSERT_OFFSETOF(summon, character, 0x1C);
    ASSERT_OFFSETOF(summon, levels,    0x20);
    ASSERT_OFFSETOF(summon, unk_34,    0x34);
    ASSERT_OFFSETOF(summon, level,     0x50);
    ASSERT_OFFSETOF(summon, unk_54,    0x54);

    ASSERT_SIZEOF  (summon_progression,                  0x30);
    ASSERT_OFFSETOF(summon_progression, changes,         0x04);
    ASSERT_OFFSETOF(summon_progression, summons,         0x18);
    ASSERT_OFFSETOF(summon_progression, summons_by_name, 0x2C);
} // treyarch
