#pragma once

#include "treyarch/game/quest_descriptions.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct progression_messages_unk_04 {
        mash::string     expression;
        u32              unk_0c;
        u32              unk_10;
        f32              unk_14;
        quest_condition* condition;

        // inlined @ sub_94C550
        void construct_mashed_class() {
            expression.construct_mashed_class();

            if (condition)
                mash::mash_info_struct::construct_class(condition);
        }

        // inlined @ sub_929080
        void destruct_mashed_class() {
            expression.destruct_mashed_class();

            if (condition) {
                condition->destruct_mashed_class();
                condition = nullptr;
            }
        }

        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class progression_messages {

    public:
        void* self; // the resource handler points this at the messages themselves

        mash::vector
            <progression_messages_unk_04> messages; // walked in order, each waiting on its condition

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (progression_messages_unk_04,            0x1C);
    ASSERT_OFFSETOF(progression_messages_unk_04, unk_0c,    0x0C);
    ASSERT_OFFSETOF(progression_messages_unk_04, condition, 0x18);

    ASSERT_SIZEOF  (progression_messages,           0x18);
    ASSERT_OFFSETOF(progression_messages, messages, 0x04);
} // treyarch
