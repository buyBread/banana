#pragma once

#include <array>

#include "vc8/vector.hh"
#include "game/wds/ai/state_graph.hh"
#include "game/boolx.hh"
#include "game/resource_key.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class ai_core;
    class ai_state_machine;
    class default_state_transition;
    class info_node_desc_list;
    class param_block;
    class state_graph_list;

    enum e_state_action_types : u32 {
        state_action_goto_state,
        state_action_return,
        state_action_machine_exit,
        state_action_no_action
    };

    // what a state's process_exit_code asks its machine to do
    struct state_trans_action {
        e_state_action_types the_action;
        string_hash          dest_state_name; // ai_state_id
        e_state_exit_code    the_exit_code;
        param_block*         extra_params;
    };

    // a machine's running state, made from its mashed_state; no retail vtable of its own, enhanced_state fills every slot
    class base_state : public mash::mash_virtual_base {

    public:
        const mashed_state*     my_mashed_state;
              ai_state_machine* my_state_machine;
        const param_block*      my_extra_params;

        explicit base_state(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        virtual void activate(      ai_state_machine* the_state_machine,
                              const mashed_state*     the_mashed_state,
                              const mashed_state*     prev_mashed_state,
                              const param_block*      extra_params) = 0;

        virtual void               deactivate(const mashed_state* next_mashed_state) = 0;
        virtual e_state_exit_code  frame_advance(f32 delta_t) = 0;
        virtual void               get_info_node_list(info_node_desc_list &the_list) const = 0;
        virtual void               get_state_graph_list(state_graph_list &the_list) const = 0;
        virtual bool               is_enhanced_state() const = 0;
        virtual bool               is_combat_state() const = 0; // true only for combat_state
        virtual bool               unk_016() const = 0;
        virtual bool               unk_017() const = 0;
        virtual state_trans_action process_exit_code(f32 delta_t, e_state_exit_code the_exit_code) const = 0;
        virtual u32                unk_019() const = 0;         // the state's category, which activate keeps in the core's state_category

        // sub_4C4940
        string_hash get_name() const {
            return my_mashed_state->get_name();
        }

        ai_core* get_core() const;
        actor*   get_actor() const;

        void initialize(mash::allocation_scope scope);

        static base_state* create_subclass(mash::virtual_types_key the_class_to_make);

        // the vtable create_subclass gives a state of source class T in place of its retail one
        template<typename T>
        static void* source_vtable() {
            return (void*)mash::mash_virtual_base::generate_vtable<T>();
        }

    private:
        template<typename F>
        static base_state* construct(u32 size, F constructor, void* vtable = nullptr); // a vtable replaces the one the retail constructor wrote
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

        explicit enhanced_state(mash::generating_vtable_version so_do_nothing) : base_state(so_do_nothing) {}

        ~enhanced_state() override;

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        void                    copy_values(const mash_virtual_base* other) override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void               deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code  frame_advance(f32 delta_t) override;
        void               get_info_node_list(info_node_desc_list &the_list) const override;
        void               get_state_graph_list(state_graph_list &the_list) const override;
        bool               is_enhanced_state() const override;
        bool               is_combat_state() const override;
        bool               unk_016() const override;
        bool               unk_017() const override;
        state_trans_action process_exit_code(f32 delta_t, e_state_exit_code the_exit_code) const override;
        u32                unk_019() const override;

        void initialize(mash::allocation_scope scope);

        bool can_handle_exit_code(e_state_exit_code the_exit_code, bool include_always = false);

        static enhanced_state* construct_inline(u32 size, void* vtable); // enhanced_state's constructor as the states without one of their own inline it
    };

    class attack_enabled_state : public enhanced_state {

    public:
        u8 reserved_044[0x110];

        explicit attack_enabled_state(mash::generating_vtable_version so_do_nothing) : enhanced_state(so_do_nothing) {}

        void                    construct_mashed_class() override;
        void                    destruct_mashed_class() override;
        void                    unmash(mash::mash_info_struct* mash_info,
                                       void*                   containing_class_ptr,
                                       mash::buffer_type       buffer) override;
        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        void activate(      ai_state_machine* the_state_machine,
                      const mashed_state*     the_mashed_state,
                      const mashed_state*     prev_mashed_state,
                      const param_block*      extra_params) override;

        void              deactivate(const mashed_state* next_mashed_state) override;
        e_state_exit_code frame_advance(f32 delta_t) override;

        virtual i32  unk_020(i32 unk);
        virtual i32  unk_021(f32 unk);
        virtual bool unk_022(i32 unk_1, i32 unk_2);
        virtual bool unk_023();
    };

    // layouts only from here on; retail builds these and their vtables stay retail

    class launch_layer_state : public enhanced_state {

    public:
        enum e_launch_state : u32 {
            waiting_for_old_machine_to_die,
            blocking_on_machine
        };

        enum e_block_level : u32 {
            launch_non_blocking,
            launch_soft_blocking,
            launch_hard_blocking
        };

        resource_key   layer_resource_key;
        e_block_level  block_level;
        e_launch_state launch_status;

        virtual resource_key  get_layer_resource_key() const; // the graph it launches
        virtual e_block_level get_block_level() const;

        static string_hash my_type_key;
    };

    class generic_loco_state : public attack_enabled_state {

    public:
        u8 reserved_154[0x04];
    };

    class jump_fly_state : public generic_loco_state {

    public:
        u8 reserved_158[0x04];
    };

    class fall_state : public jump_fly_state {

    public:
        u8 reserved_15c[0x24];
    };

    class zip_fall_state : public fall_state {

    public:
        bool unk_180;
        u8   reserved_181[0x0F];
        bool unk_190;
        u8   reserved_191[0x07];
    };

    class force_drive_car_state : public enhanced_state {

    public:
        boolx car_combat_stop_car;
        u8    pad_045[0x03];
    };

    class drive_car_state : public force_drive_car_state {};

    class plr_swim_state : public enhanced_state {

    public:
        boolx can_dodge;
        u8    pad_045[0x03];
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

        // sub_9EEB90
        ai_core* get_core() const {
            return my_core;
        }

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
        void process_return();

        bool transition_state(string_hash new_state_id, const param_block* extra_params = nullptr, e_state_exit_code the_exit_code = state_exit_code_no_exit);
    };

    ASSERT_OFFSETOF(base_state, my_mashed_state,  0x04);
    ASSERT_OFFSETOF(base_state, my_state_machine, 0x08);
    ASSERT_OFFSETOF(base_state, my_extra_params,  0x0C);

    ASSERT_SIZEOF  (state_trans_action,                0x10);
    ASSERT_OFFSETOF(state_trans_action, the_exit_code, 0x08);

    ASSERT_SIZEOF  (enhanced_state,                     0x44);
    ASSERT_OFFSETOF(enhanced_state, timeout_exit_code,  0x18);
    ASSERT_OFFSETOF(enhanced_state, exit_codes_checked, 0x24);
    ASSERT_OFFSETOF(enhanced_state, exit_codes_handled, 0x34);

    ASSERT_SIZEOF  (launch_layer_state,                     0x54);
    ASSERT_OFFSETOF(launch_layer_state, layer_resource_key, 0x44);

    ASSERT_SIZEOF(attack_enabled_state, 0x154);
    ASSERT_SIZEOF(generic_loco_state,   0x158);
    ASSERT_SIZEOF(jump_fly_state,       0x15C);
    ASSERT_SIZEOF(fall_state,           0x180);

    ASSERT_SIZEOF  (zip_fall_state,          0x198);
    ASSERT_OFFSETOF(zip_fall_state, unk_180, 0x180);
    ASSERT_OFFSETOF(zip_fall_state, unk_190, 0x190);

    ASSERT_SIZEOF  (force_drive_car_state,                      0x48);
    ASSERT_OFFSETOF(force_drive_car_state, car_combat_stop_car, 0x44);
    ASSERT_SIZEOF  (drive_car_state,                            0x48);

    ASSERT_SIZEOF  (plr_swim_state,            0x48);
    ASSERT_OFFSETOF(plr_swim_state, can_dodge, 0x44);

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
        inline util::memory_reference<std::array<string_hash, state_exit_code_no_exit>> event_hashes      { 0x00FC5D10 }; // raise_event_on_<code>

        inline util::memory_reference<string_hash> to_state_always_hash   { 0x00FC55B0 };
        inline util::memory_reference<string_hash> exit_layer_always_hash { 0x00FC5D08 };

        // process_mode's, made on first use
        inline util::memory_reference<string_hash> sync_state_exit_code       { 0x00FC5EB8 };
        inline util::memory_reference<u32>         sync_state_exit_code_guard { 0x00FC5EBC };

        // the states create_subclass constructs inline
        inline util::memory_reference<void*> civ_beat_up_goto_state_vtable              { 0x00B958AC };
        inline util::memory_reference<void*> drive_car_state_vtable                     { 0x00B93A9C };
        inline util::memory_reference<void*> enhanced_state_vtable                      { 0x00B8A684 };
        inline util::memory_reference<void*> force_drive_car_state_vtable               { 0x00B93A44 };
        inline util::memory_reference<void*> generic_wait_for_movement_state_vtable     { 0x00B91DF4 };
        inline util::memory_reference<void*> go_to_abductee_state_vtable                { 0x00B956D4 };
        inline util::memory_reference<void*> go_to_car_state_vtable                     { 0x00B93814 };
        inline util::memory_reference<void*> go_to_cover_state_vtable                   { 0x00B944CC };
        inline util::memory_reference<void*> go_to_harassment_victim_state_vtable       { 0x00B95D74 };
        inline util::memory_reference<void*> go_to_hostage_state_vtable                 { 0x00B95FAC };
        inline util::memory_reference<void*> going_to_patrol_node_state_vtable          { 0x00B92D64 };
        inline util::memory_reference<void*> ped_flee_state_vtable                      { 0x00B93EC4 };
        inline util::memory_reference<void*> ped_fleeing_state_vtable                   { 0x00B93F1C };
        inline util::memory_reference<void*> ped_watch_state_vtable                     { 0x00B93F74 };
        inline util::memory_reference<void*> plr_agility_test_state_vtable              { 0x00B947DC };
        inline util::memory_reference<void*> plr_car_combat_state_vtable                { 0x00B94F64 };
        inline util::memory_reference<void*> plr_loco_crawl_state_vtable                { 0x00B952CC };
        inline util::memory_reference<void*> plr_swim_state_vtable                      { 0x00B94F0C };
        inline util::memory_reference<void*> puppet_movement_state_vtable               { 0x00B91FD4 };
        inline util::memory_reference<void*> raise_event_state_vtable                   { 0x00B91F04 };
        inline util::memory_reference<void*> slave_state_vtable                         { 0x00B923FC };
        inline util::memory_reference<void*> swift_attach_to_wall_state_vtable          { 0x00B933FC };
        inline util::memory_reference<void*> swift_crawl_transition_state_vtable        { 0x00B9332C };
        inline util::memory_reference<void*> swift_find_and_attach_to_wall_state_vtable { 0x00B93454 };
        inline util::memory_reference<void*> swift_fly_locomotion_state_vtable          { 0x00B935EC };
        inline util::memory_reference<void*> swift_locomotion_state_vtable              { 0x00B9318C };
        inline util::memory_reference<void*> swift_obstacle_avoid_state_vtable          { 0x00B9326C };
        inline util::memory_reference<void*> swift_obstacle_break_state_vtable          { 0x00B91AC4 };
        inline util::memory_reference<void*> swift_stationary_state_vtable              { 0x00B934AC };
        inline util::memory_reference<void*> swift_wall_locomotion_state_vtable         { 0x00B931FC };
        inline util::memory_reference<void*> teleporting_locomotion_state_vtable        { 0x00B93504 };
        inline util::memory_reference<void*> unmoving_state_vtable                      { 0x00B94784 };
        inline util::memory_reference<void*> wait_for_car_stop_state_vtable             { 0x00B939EC };
        inline util::memory_reference<void*> wander_state_vtable                        { 0x00B93E6C };
        inline util::memory_reference<void*> zip_fall_state_vtable                      { 0x00B924F4 };
    } // references
} // treyarch
