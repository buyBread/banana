#pragma once

#include "treyarch/game/wds/ai/ai_team.hh"
#include "treyarch/game/wds/ai/info_node.hh"
#include "treyarch/game/wds/ai/param_block.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace slim {
        class slim_actor_controller;
    } // slim

    class actor;
    class ai_core;
    class ai_state_machine;
    class core_ai_resource;
    class loco_inode;

    enum e_ai_core_flags : u32 {
        ai_core_flag_forcing_high_priority = 0x00000001,
        ai_core_flag_ignore_limbo          = 0x00000002,
        ai_core_flag_in_limbo              = 0x00000004,

        ai_core_flag_unk_00000008 = 0x00000008, // with the civilian team, demotes the core to the low list
        ai_core_flag_unk_00000100 = 0x00000100
    };

    enum e_ai_core_mode : u32 {
        ai_core_mode_normal,
        ai_core_mode_change_machines,
        ai_core_mode_killing_machines
    };

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
            dinkumware::vector
                <dinkumware::vector<ai_core*>> teams;

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
        dinkumware::list<void*>         active_records;       // entries point into records
        u8                              reserved_5bc[0x0C];
        u8                              reserved_5c8[0x0C];
        ai_state_machine*               my_base_machine;
        ai_state_machine*               my_locomotion_machine;
        dinkumware::list<sm_list_entry> my_machine_list;
        u8                              reserved_5e8[0x0C];
        e_ai_core_mode                  my_mode;
        resource_key                    new_base_machine;
        e_ai_core_mode                  my_locomotion_mode;
        loco_inode*                     my_loco_inode;
        void*                           path_location;
        u32                             locomotion_exit_status;
        u32                             unk_610;
        ai_team::e_team                 my_team;              // num_teams when the "team" param is missing or unknown
        bool                            alive;
        u8                              pad_619[0x03];
        u32                             state_category;
        u32                             flags;
        u8                              reserved_624[0x10];
        u32                             lifetime_effect;
        u32                             unk_638;
        u8                              reserved_63c[0x24];
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
    ASSERT_OFFSETOF(ai_core, my_base_machine,        0x5D4);
    ASSERT_OFFSETOF(ai_core, my_machine_list,        0x5DC);
    ASSERT_OFFSETOF(ai_core, my_mode,                0x5F4);
    ASSERT_OFFSETOF(ai_core, new_base_machine,       0x5F8);
    ASSERT_OFFSETOF(ai_core, my_locomotion_mode,     0x600);
    ASSERT_OFFSETOF(ai_core, my_loco_inode,          0x604);
    ASSERT_OFFSETOF(ai_core, locomotion_exit_status, 0x60C);
    ASSERT_OFFSETOF(ai_core, my_team,                0x614);
    ASSERT_OFFSETOF(ai_core, alive,                  0x618);
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
