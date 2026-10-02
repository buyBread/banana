#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    // idk the original class name; it holds the level's "global" environment_progression resource (type 71)
    class environment_progression_state {

    public:
        void* environment_progression; // set by sub_7EB4D0 during game::load_this_level
        u32   unk_04;
        i32   unk_08;                  // -1 until set
        f32   progression_level;       // what the get_progression_level native returns

        environment_progression_state();

        static void create_inst();
    };

    namespace references {
        inline util::memory_reference<environment_progression_state*> environment_progression_state { 0x0108805C };
    } // references

    ASSERT_SIZEOF  (environment_progression_state,                    0x10);
    ASSERT_OFFSETOF(environment_progression_state, progression_level, 0x0C);
} // treyarch
