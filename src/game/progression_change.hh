#pragma once

#include "game/quest_descriptions.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // applied whenever its condition holds: added names switch on, removed ones off ("ALL" removes everything)
    struct progression_change {
        mash::string       name;
        mash::string       expression;
        mash::vector
            <mash::string> added;
        mash::vector
            <mash::string> removed;
        quest_condition*   condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (progression_change,            0x44);
    ASSERT_OFFSETOF(progression_change, added,     0x18);
    ASSERT_OFFSETOF(progression_change, removed,   0x2C);
    ASSERT_OFFSETOF(progression_change, condition, 0x40);
} // treyarch
