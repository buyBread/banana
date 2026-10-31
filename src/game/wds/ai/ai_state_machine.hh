#pragma once

#include <array>

#include "vc8/vector.hh"
#include "game/wds/ai/state_graph.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_core;
    class ai_state_machine;
    class default_state_transition;
    class info_node_desc_list;
    class param_block;
    class state_graph_list;

    // a machine's running state, made from its mashed_state
    class base_state : public mash::mash_virtual_base {

    public:
        const mashed_state*     my_mashed_state;
              ai_state_machine* my_state_machine;
        const param_block*      my_extra_params;

        virtual void activate(      ai_state_machine* the_state_machine,
                              const mashed_state*     the_mashed_state,
                              const mashed_state*     prev_mashed_state,
                              const param_block*      extra_params);

        virtual void              deactivate(const mashed_state* next_mashed_state);
        virtual e_state_exit_code frame_advance(f32 delta_t) = 0;
        virtual void              get_info_node_list(info_node_desc_list &the_list) const;
        virtual void              get_state_graph_list(state_graph_list &the_list) const;
        virtual bool              is_enhanced_state() const;
        virtual bool              is_combat_state() const; // true only for combat_state

        // sub_4C4940
        string_hash get_name() const {
            return my_mashed_state->get_name();
        }
    };

    class enhanced_state : public base_state {

    public:
        f32                              state_timer;
        i32                              state_start_frame;
        e_state_exit_code                timeout_exit_code;
        vc8::vector
            <default_state_transition*>* default_transitions;
        param_block*                     params_for_next_state;
        u32                              exit_codes_checked[4]; // bit n is set once can_handle_exit_code has answered for code n
        u32                              exit_codes_handled[4]; // and here when the answer was yes

        bool can_handle_exit_code(e_state_exit_code the_exit_code, bool include_always = false);
    };

    class ai_state_machine {

    public:
        enum operation_mode : u32 {
            initial_mode,
            state_advance,
            blocked_on_child_machine,
            blocked_on_all_children,
            processing_exit_request,
            exiting_wait_on_children
        };

              operation_mode          my_curr_mode;
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
              e_state_exit_code       my_exit_status;

        // sub_4C6880
        i32 get_num_children() const {
            return (i32)my_children.size();
        }

        // sub_4C6900
        string_hash get_curr_state_name() const { // "invalid" without a current state
            if (my_curr_state)
                return my_curr_state->get_name();

            string_hash name;
            name.initialize(mash::ALLOCATED, "invalid");

            return name;
        }

        resource_key get_name() const;

        bool can_switch_to_state(string_hash id) const;

        void request_exit();
        void external_request_exit(); // leaves a machine that's already exiting alone

        void process_mode(f32 delta_t);
        void advance_curr_state(f32 delta_t);
        void process_exit_code(f32 delta_t, e_state_exit_code the_exit_code);
    };

    ASSERT_OFFSETOF(base_state, my_mashed_state,  0x04);
    ASSERT_OFFSETOF(base_state, my_state_machine, 0x08);
    ASSERT_OFFSETOF(base_state, my_extra_params,  0x0C);

    ASSERT_SIZEOF  (enhanced_state,                     0x44);
    ASSERT_OFFSETOF(enhanced_state, timeout_exit_code,  0x18);
    ASSERT_OFFSETOF(enhanced_state, exit_codes_checked, 0x24);
    ASSERT_OFFSETOF(enhanced_state, exit_codes_handled, 0x34);

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

    namespace references {
        // enhanced_state's param names, one per exit code
        inline util::memory_reference<std::array<string_hash, state_exit_code_no_exit>> to_state_hashes   { 0x00FC5320 };
        inline util::memory_reference<std::array<string_hash, state_exit_code_no_exit>> exit_layer_hashes { 0x00FC57F0 };

        inline util::memory_reference<string_hash> to_state_always_hash   { 0x00FC55B0 };
        inline util::memory_reference<string_hash> exit_layer_always_hash { 0x00FC5D08 };

        // process_mode's, made on first use
        inline util::memory_reference<string_hash> sync_state_exit_code       { 0x00FC5EB8 };
        inline util::memory_reference<u32>         sync_state_exit_code_guard { 0x00FC5EBC };
    } // references
} // treyarch
