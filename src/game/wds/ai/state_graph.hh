#pragma once

#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a state's data in its graph; the running state is a base_state made from my_state_type
    class alignas(8) mashed_state {

    public:
        enum e_base_state_flags : u32 {
            is_interrupt_state = 0
        };

        param_block                   my_params;
        string_hash                   my_name;  // ai_state_id
        u32                           my_flags; // bits by e_base_state_flags
        mash::virtual_types_key       my_state_type;
        mash::vector_basic
            <mash::virtual_types_key> my_default_state_transitions;
        u8                            pad_03c[0x04];

        // sub_72C640
        string_hash get_name() const {
            return my_name;
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class state_graph {

    public:
        void* self; // the resource handler points this at the graph itself

        resource_key       my_name;
        mash::vector
            <mashed_state> my_states;
        mashed_state*      my_initial_state; // mashed on its own, not one of my_states

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (mashed_state,                               0x40);
    ASSERT_OFFSETOF(mashed_state, my_name,                      0x20);
    ASSERT_OFFSETOF(mashed_state, my_state_type,                0x28);
    ASSERT_OFFSETOF(mashed_state, my_default_state_transitions, 0x2C);

    ASSERT_SIZEOF  (state_graph,                   0x24);
    ASSERT_OFFSETOF(state_graph, my_name,          0x04);
    ASSERT_OFFSETOF(state_graph, my_states,        0x0C);
    ASSERT_OFFSETOF(state_graph, my_initial_state, 0x20);
} // treyarch
