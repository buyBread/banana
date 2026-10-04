#pragma once

#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class ai_core;
    class base_state;
    class mashed_state;
    class param_block;
    class state_graph;

    class ai_state_machine {

    public:
              u32                     my_curr_mode;
              ai_core*                my_core;
        const state_graph*            my_graph;
              base_state*             my_curr_state;
        const mashed_state*           my_prev_state;
              ai_state_machine*       my_parent;
              dinkumware::vector
                  <ai_state_machine*> my_children;
              ai_state_machine*       my_blocked_machine;
              void*                   my_memory_block;
        const mashed_state*           my_interrupted_state;
        const param_block*            my_interrupted_params;
              u32                     my_exit_status;
    };

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
