#pragma once

#include "vc8/vector.hh"
#include "game/wds/ai/state_graph.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_core;
    class param_block;

    // a machine's running state, made from its mashed_state
    class base_state : public mash::mash_virtual_base {

    public:
        const mashed_state* my_mashed_state;

        // sub_4C4940
        string_hash get_name() const {
            return my_mashed_state->get_name();
        }

        // slot 15; true only for combat_state
        bool is_combat_state() const {
            return ((bool (__thiscall*)(const base_state*))(*(void***)this)[15])(this);
        }
    };

    class ai_state_machine {

    public:
              u32                     my_curr_mode;
              ai_core*                my_core;
        const state_graph*            my_graph;
              base_state*             my_curr_state;
        const mashed_state*           my_prev_state;
              ai_state_machine*       my_parent;
              vc8::vector
                  <ai_state_machine*> my_children;
              ai_state_machine*       my_blocked_machine;
              void*                   my_memory_block;
        const mashed_state*           my_interrupted_state;
        const param_block*            my_interrupted_params;
              u32                     my_exit_status;

        // sub_4C6900
        string_hash get_curr_state_name() const { // "invalid" without a current state
            if (my_curr_state)
                return my_curr_state->get_name();

            string_hash name;
            name.initialize(mash::ALLOCATED, "invalid");

            return name;
        }
    };

    ASSERT_OFFSETOF(base_state, my_mashed_state, 0x04);

    ASSERT_SIZEOF  (ai_state_machine,                        0x3C);
    ASSERT_OFFSETOF(ai_state_machine, my_core,               0x04);
    ASSERT_OFFSETOF(ai_state_machine, my_graph,              0x08);
    ASSERT_OFFSETOF(ai_state_machine, my_curr_state,         0x0C);
    ASSERT_OFFSETOF(ai_state_machine, my_parent,             0x14);
    ASSERT_OFFSETOF(ai_state_machine, my_children,           0x18);
    ASSERT_OFFSETOF(ai_state_machine, my_blocked_machine,    0x28);
    ASSERT_OFFSETOF(ai_state_machine, my_memory_block,       0x2C);
    ASSERT_OFFSETOF(ai_state_machine, my_interrupted_params, 0x34);
    ASSERT_OFFSETOF(ai_state_machine, my_exit_status,        0x38);
} // treyarch
