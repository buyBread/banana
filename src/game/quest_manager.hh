#pragma once

#include "treyarch/mash/string.hh"
#include "game/math/algebra.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;
    struct quest_description;
    struct quest_giver;

    // one per quest, in quest order
    struct quest_state {
        i32 progress[4]; // one counter per quest element
        i32 unk_10[4];
        u32 status;      // 4 in progress, 8 completed; available quests have 2 or 0x20
        f32 timer;
    };

    class quest_manager : public singleton_instance<quest_manager, 0x01087FD4> {

    public:
        u8                  unk_000;
        u8                  reserved_001[0x03];
        i32                 n_quests;
        quest_description** quests;
        quest_state*        quest_states;
        i32                 giver_count;
        quest_giver**       givers;
        u8                  reserved_018[0x1C];
        bool                progression_update_pending; // the tick re-checks the unlock rules once and clears it

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

        static void draw_text();
    };

    ASSERT_SIZEOF  (quest_state,         0x28);
    ASSERT_OFFSETOF(quest_state, status, 0x20);
    ASSERT_OFFSETOF(quest_state, timer,  0x24);

    ASSERT_OFFSETOF(quest_manager, n_quests,                   0x04);
    ASSERT_OFFSETOF(quest_manager, quest_states,               0x0C);
    ASSERT_OFFSETOF(quest_manager, givers,                     0x14);
    ASSERT_OFFSETOF(quest_manager, progression_update_pending, 0x34);

    namespace references {
        // created by sub_7F7760 during game::load_this_level
        inline util::memory_reference<igo_3d_text*> quest_text { 0x01087FCC };

        // the QUEST devopt COMPLETE_ACTIVE_QUESTS; the quest manager's tick clears it
        inline util::memory_reference<u8> complete_active_quests { 0x00E82360 };
    } // references
} // treyarch
