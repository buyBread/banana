#pragma once

#include "vc8/list.hh"
#include "vc8/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class conversation;
    class entity;
    class event;
    class igo_3d_text;
    class trigger;
    struct quest_description;
    struct quest_giver;

    // one per quest, in quest order
    struct quest_state {
        i32 progress[4]; // one counter per quest element
        i32 unk_10[4];
        u32 status;      // 4 in progress, 8 completed; available quests have 2 or 0x20
        f32 timer;
    };

    // a registered quest giver entity
    struct giver_entity {
        actor*        entity;
        trigger*      sphere_trigger;             // around the giver; entering it puts up the square button
        conversation* attached_conversation;      // attach_conversation's
        mash::string  name;                       // the quest giver's whose name hashes to the entity's id
        bool          unk_18;                     // a square press that starts no conversation offers the giver's quests instead
        u8            pad_19[0x03];
        i32           face_button;
        u32           square_pressed_callback_id;
    };

    class quest_manager : public singleton_instance<quest_manager, 0x01087FD4> {

    public:
        bool                        hold_blackscreen;           // the tick lifts the blackscreen once streaming settles, then clears it
        bool                        wait_for_idle_mission;      // the tick waits for an idle mission manager while it's set
        u8                          reserved_002[0x02];
        i32                         n_quests;
        quest_description**         quests;
        quest_state*                quest_states;
        i32                         giver_count;
        quest_giver**               givers;
        u8                          reserved_018[0x1C];
        bool                        progression_update_pending; // the tick re-checks the unlock rules once and clears it
        u8                          reserved_035[0x4B];
        i32                         unk_080;                    // the tick reports the quest's completion to the mission manager; -1 none
        vc8::vector<giver_entity*>* giver_entities;
        u8                          reserved_088[0x204];
        vc8::list<mash::string>     deferred_missions;          // launched by name once no mission runs
        entity*                     suspended_entity;           // the giver being talked to

        void         frame_advance(f32 time_inc);

        void         update_progression();
        void         set_completed_quest_timers();
        void         give_quest(const mash::string&);

        i32          available_quest_count();
        mash::string get_available_quest(i32 n);
        bool         check_completion(i32 index);
        i32          completed_quest_count();
        mash::string get_completed_quest(i32 n);
        i32          get_quest_index(const mash::string &name);

        mash::string get_localized_giver_name(const mash::string &giver);
        mash::string get_questgiver_name(const mash::string &name);
        vector3d     get_questgiver_loc(const mash::string &name);

        void         giver_register_entity(actor* giver, bool unk);
        static void  giver_trigger_entered(event* raised_event, arch_base_vhandle recipient, void* parameters); // puts up the square button
        static void  giver_square_pressed (event* raised_event, arch_base_vhandle recipient, void* parameters); // starts the giver's conversation

        static void  draw_text();
    };

    ASSERT_SIZEOF  (quest_state,         0x28);
    ASSERT_OFFSETOF(quest_state, status, 0x20);
    ASSERT_OFFSETOF(quest_state, timer,  0x24);

    ASSERT_SIZEOF  (giver_entity,                             0x24);
    ASSERT_OFFSETOF(giver_entity, attached_conversation,      0x08);
    ASSERT_OFFSETOF(giver_entity, name,                       0x0C);
    ASSERT_OFFSETOF(giver_entity, unk_18,                     0x18);
    ASSERT_OFFSETOF(giver_entity, face_button,                0x1C);
    ASSERT_OFFSETOF(giver_entity, square_pressed_callback_id, 0x20);

    ASSERT_OFFSETOF(quest_manager, wait_for_idle_mission,      0x001);
    ASSERT_OFFSETOF(quest_manager, n_quests,                   0x004);
    ASSERT_OFFSETOF(quest_manager, quest_states,               0x00C);
    ASSERT_OFFSETOF(quest_manager, givers,                     0x014);
    ASSERT_OFFSETOF(quest_manager, progression_update_pending, 0x034);
    ASSERT_OFFSETOF(quest_manager, unk_080,                    0x080);
    ASSERT_OFFSETOF(quest_manager, giver_entities,             0x084);
    ASSERT_OFFSETOF(quest_manager, deferred_missions,          0x28C);
    ASSERT_OFFSETOF(quest_manager, suspended_entity,           0x298);

    namespace references {
        // created by sub_7F7760 during game::load_this_level
        inline util::memory_reference<igo_3d_text*> quest_text { 0x01087FCC };

        // the QUEST devopts COMPLETE_ACTIVE_QUESTS and PICK_UP_AVAILABLE_QUESTS; the quest manager's tick clears them
        inline util::memory_reference<u8> complete_active_quests   { 0x00E82360 };
        inline util::memory_reference<u8> pick_up_available_quests { 0x00E82380 };

        inline util::memory_reference<string_hash> square_pressed { 0x0102C67C };
    } // references
} // treyarch
