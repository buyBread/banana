#pragma once

#include "vc8/list.hh"
#include "vc8/vector.hh"
#include "game/wds/ai/ai_team.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/ai/param_block.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/mutex.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace slim {
        class slim_actor_controller;
    } // slim

    class actor;
    class ai_core;
    class ai_state_machine;
    class core_ai_resource;

    enum e_ai_core_flags : u32 {
        ai_core_flag_forcing_high_priority = 0x00000001,
        ai_core_flag_ignore_limbo          = 0x00000002,
        ai_core_flag_in_limbo              = 0x00000004,

        ai_core_flag_unk_00000008 = 0x00000008, // with the civilian team, demotes the core to the low list
        ai_core_flag_unk_00000100 = 0x00000100,

        ai_core_flag_seated_in_car = 0x00010000 // the actor's base als state says so
    };

    enum e_ai_core_mode : u32 {
        ai_core_mode_normal,
        ai_core_mode_change_machines,
        ai_core_mode_killing_machines
    };

    // what a state machine finished with; 106 codes in all
    enum e_state_exit_code : u32 {
        state_exit_code_exit_request = 0,
        state_exit_code_success      = 1,
        state_exit_code_failure      = 2,
        state_exit_code_no_exit      = 106
    };

    inline const string_hash tether_active_key = string_hash(hash::djb2("TETHER_ACTIVE"));
    inline const string_hash tether_center_key = string_hash(hash::djb2("TETHER_CENTER"));
    inline const string_hash tether_radius_key = string_hash(hash::djb2("TETHER_RADIUS"));

    struct ai_core_list;

    // every core owns one node and sits on exactly one of the two global lists
    struct ai_core_list_node {
        ai_core_list_node* next;
        ai_core_list_node* previous;
        ai_core_list*      list;
        ai_core*           core;
    };

    struct ai_core_list {
        ai_core_list_node* head;
        ai_core_list_node* tail;
        u32                count;

        static void remove(ai_core_list_node* node);
        void        push_back(ai_core_list_node* node);
    };

    ASSERT_SIZEOF(ai_core_list_node, 0x10);
    ASSERT_SIZEOF(ai_core_list,      0x0C);

    struct sm_list_entry {
        ai_state_machine* my_machine;
        bool              need_to_delete;
        u8                pad_005[0x03];
    };

    class ai_core {

    public:
        // per-frame index of the live cores by team
        class team_lists {

        public:
            bool                               valid;
            u8                                 pad_001[0x03];
            vc8::vector
                <vc8::vector<ai_core*>> teams;

            void clear();
            void rebuild();
        };

        u32                             unk_000;
        engine_recursive_lock*          lock;
        u8                              reserved_008[0x18];
        slim::slim_actor_controller*    base_controller;
        slim::slim_actor_controller*    strategy_controller;
        slim::slim_actor_controller*    tactics_controller;
        u32                             record_slot_mask;     // bit n is set while records[n] is in use
        u8                              records[32][0x2C];
        vc8::list<void*>                active_records;       // entries point into records
        u32                             unk_5bc;              // the als's ai_idle_type; the cut scene player sets it while it has the hero
        u32                             unk_5c0;              // the als's ai_walk_run_cycle
        u32                             unk_5c4;              // the als's ai_walk_item
        u8                              reserved_5c8[0x0C];
        ai_state_machine*               my_base_machine;
        ai_state_machine*               my_locomotion_machine;
        vc8::list<sm_list_entry>        my_machine_list;
        u8                              reserved_5e8[0x0C];
        e_ai_core_mode                  my_mode;
        resource_key                    new_base_machine;
        e_ai_core_mode                  my_locomotion_mode;
        loco_inode*                     my_loco_inode;
        void*                           path_location;
        e_state_exit_code               locomotion_exit_status;
        u32                             unk_610;
        ai_team::e_team                 my_team;              // num_teams when the "team" param is missing or unknown
        bool                            alive;
        u8                              pad_619[0x03];
        u32                             state_category;
        u32                             flags;
        u8                              reserved_624[0x10];
        u32                             lifetime_effect;
        u32                             unk_638;
        bool                            unk_63c;              // the als's on_fire
        u8                              pad_63d[0x07];
        u32                             unk_644;              // the als's in_water when set
        u8                              reserved_648[0x0C];
        u32                             unk_654;              // the als's wading_water when set
        u8                              reserved_658[0x08];
        param_block                     my_param_block;
        bool                            unk_680;
        u8                              pad_681[0x03];
        mash::vector<info_node>*        my_info_node_list;
        u32                             info_node_mask[2];    // bit n is set while the node of type n exists
        actor*                          my_actor;
        void*                           unk_694;
        core_ai_resource*               my_resource;
        ai_core_list_node*              list_node;
        u8                              reserved_6a0[0x44];
        i32                             unk_6e4;

        info_node* get_info_node(e_info_node_type type) const;

        // sub_4C2FA0
        actor* get_actor(i32 actor_ix = 0) const { return my_actor; }

        bool is_moving() const { return locomotion_exit_status == state_exit_code_no_exit; }

        bool     is_tether_active() const;
        f32      get_tether_radius() const;
        vector3d get_tether_center() const;
        void     enable_tether(const vector3d &center, f32 radius);
        void     disable_tether();
        vector3d clip_to_tether(const vector3d* pos, bool* clipped) const; // pulls pos inside the tether
        bool     is_within_tether(const vector3d* pos, f32 margin) const;  // true without an active tether

        vector3d get_movement_dest() const;
        bool     set_facing_dir(const vector3d &dir, f32 rot_speed_deg);
        bool     set_facing_point(const vector3d &point, f32 rot_speed_deg);
        bool     stop_movement();

        void enter_limbo();
        void exit_limbo();

        static void frame_advance_all_core_ais(f32 delta_t);

    private:
        bool should_be_in_limbo() const;
        bool wants_low_priority() const;
        void refresh_team();
        void refresh_alive();
    };

    ASSERT_SIZEOF  (ai_core,                         0x6E8);
    ASSERT_OFFSETOF(ai_core, lock,                   0x004);
    ASSERT_OFFSETOF(ai_core, base_controller,        0x020);
    ASSERT_OFFSETOF(ai_core, tactics_controller,     0x028);
    ASSERT_OFFSETOF(ai_core, record_slot_mask,       0x02C);
    ASSERT_OFFSETOF(ai_core, records,                0x030);
    ASSERT_OFFSETOF(ai_core, active_records,         0x5B0);
    ASSERT_OFFSETOF(ai_core, unk_5bc,                0x5BC);
    ASSERT_OFFSETOF(ai_core, unk_5c4,                0x5C4);
    ASSERT_OFFSETOF(ai_core, my_base_machine,        0x5D4);
    ASSERT_OFFSETOF(ai_core, my_machine_list,        0x5DC);
    ASSERT_OFFSETOF(ai_core, my_mode,                0x5F4);
    ASSERT_OFFSETOF(ai_core, new_base_machine,       0x5F8);
    ASSERT_OFFSETOF(ai_core, my_locomotion_mode,     0x600);
    ASSERT_OFFSETOF(ai_core, my_loco_inode,          0x604);
    ASSERT_OFFSETOF(ai_core, locomotion_exit_status, 0x60C);
    ASSERT_OFFSETOF(ai_core, my_team,                0x614);
    ASSERT_OFFSETOF(ai_core, alive,                  0x618);
    ASSERT_OFFSETOF(ai_core, unk_63c,                0x63C);
    ASSERT_OFFSETOF(ai_core, unk_644,                0x644);
    ASSERT_OFFSETOF(ai_core, unk_654,                0x654);
    ASSERT_OFFSETOF(ai_core, state_category,         0x61C);
    ASSERT_OFFSETOF(ai_core, flags,                  0x620);
    ASSERT_OFFSETOF(ai_core, lifetime_effect,        0x634);
    ASSERT_OFFSETOF(ai_core, my_param_block,         0x660);
    ASSERT_OFFSETOF(ai_core, my_info_node_list,      0x684);
    ASSERT_OFFSETOF(ai_core, info_node_mask,         0x688);
    ASSERT_OFFSETOF(ai_core, my_actor,               0x690);
    ASSERT_OFFSETOF(ai_core, my_resource,            0x698);
    ASSERT_OFFSETOF(ai_core, list_node,              0x69C);
    ASSERT_OFFSETOF(ai_core, unk_6e4,                0x6E4);

    ASSERT_SIZEOF  (ai_core::team_lists,        0x14);
    ASSERT_OFFSETOF(ai_core::team_lists, teams, 0x04);

    namespace references {
        inline util::memory_reference<ai_core_list> ai_core_list_high { 0x00FC5998 };
        inline util::memory_reference<ai_core_list> ai_core_list_low  { 0x00FC59BC };

        inline util::memory_reference<ai_core::team_lists> team_lists { 0x00FC54EC };

        inline util::memory_reference<engine_recursive_lock> ai_core_list_lock { 0x00FC5C60 };
    } // references
} // treyarch
