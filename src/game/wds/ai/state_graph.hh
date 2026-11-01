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
    // what a state or a machine finished with; 106 codes in all
    enum e_state_exit_code : u32 {
        state_exit_code_exit_request        = 0,
        state_exit_code_success             = 1,
        state_exit_code_failure             = 2,
        state_exit_code_attack              = 15,
        state_exit_code_hit_switch          = 16,
        state_exit_code_ranged_hit_switch   = 17,
        state_exit_code_damaged             = 18,
        state_exit_code_exploded            = 19,
        state_exit_code_death               = 21,
        state_exit_code_biped_physics       = 22,
        state_exit_code_transition          = 28,
        state_exit_code_take_hostage        = 44,
        state_exit_code_rappel              = 54,
        state_exit_code_stand               = 59,
        state_exit_code_jump                = 60,
        state_exit_code_land                = 61,
        state_exit_code_fall                = 62,
        state_exit_code_crawl               = 63,
        state_exit_code_ground_splat        = 70,
        state_exit_code_swing               = 73,
        state_exit_code_zip                 = 75,
        state_exit_code_wall_sprint         = 76,
        state_exit_code_wall_grind          = 77,
        state_exit_code_pole_swing          = 78,
        state_exit_code_yoyo                = 79,
        state_exit_code_strength_test       = 81,
        state_exit_code_dodge               = 84,
        state_exit_code_block               = 85,
        state_exit_code_swim                = 86,
        state_exit_code_wall_evade          = 91,
        state_exit_code_prop_throw          = 93,
        state_exit_code_jump_launch         = 95,
        state_exit_code_obstacle_hop        = 96,
        state_exit_code_sync_obstacle_avoid = 98,
        state_exit_code_no_exit             = 106
    };

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

        bool is_flag_set(e_base_state_flags the_flag) const {
            return my_flags & (1u << the_flag);
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

        // sub_4C2BB0
        resource_key get_name() const {
            resource_key name;
            name.set(my_name);

            return name;
        }

        const mashed_state* find_state(string_hash the_state_name) const; // null when the graph has no such state

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
