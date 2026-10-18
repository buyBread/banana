#pragma once

#include "treyarch/game/quest_descriptions.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct environment_progression_stage {
        mash::string     name;
        mash::string     expression;
        f32              unk_18;
        i32              unk_1c;
        quest_condition* condition;

        // inlined @ sub_7FFCE0
        void construct_mashed_class() {
            name      .construct_mashed_class();
            expression.construct_mashed_class();

            if (condition)
                mash::mash_info_struct::construct_class(condition);
        }

        // inlined @ sub_7F8AD0
        void destruct_mashed_class() {
            name      .destruct_mashed_class();
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

    class environment_progression {

    public:
        void* self; // the resource handler points this at the progression itself

        mash::vector<environment_progression_stage> stages;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    // idk the original class name; it holds the level's "global" environment_progression resource (type 71)
    class environment_progression_state {

    public:
        environment_progression* progression; // set by sub_7EB4D0 during game::load_this_level
        u32                      unk_04;
        i32                      unk_08;            // -1 until set
        f32                      progression_level; // what the get_progression_level native returns

        environment_progression_state();

        static void create_inst();
    };

    namespace references {
        inline util::memory_reference<environment_progression_state*> environment_progression_state { 0x0108805C };
    } // references

    ASSERT_SIZEOF  (environment_progression_stage,            0x24);
    ASSERT_OFFSETOF(environment_progression_stage, unk_18,    0x18);
    ASSERT_OFFSETOF(environment_progression_stage, condition, 0x20);

    ASSERT_SIZEOF  (environment_progression,         0x18);
    ASSERT_OFFSETOF(environment_progression, stages, 0x04);

    ASSERT_SIZEOF  (environment_progression_state,                    0x10);
    ASSERT_OFFSETOF(environment_progression_state, progression_level, 0x0C);
} // treyarch
