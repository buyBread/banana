#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_list_box.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_text_scrollable.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class conversation;
    class sound_response;

    namespace references {
        // development_option<int> Conversation/menu_type, 3 in retail data
        inline util::memory_reference<i32> conversation_menu_type { 0x0102F310 };

        inline util::memory_reference<string_hash> conversation_begin  { 0x0102C620 };
        inline util::memory_reference<string_hash> conversation_cancel { 0x0102C6C4 };
    } // references

    class conversation_menu_system : public igo_generic_input_listener, public igo_3d_widget {

    public:
        vc8::vector<u32>        unk_160;
        f32                     text_scroll_time;      // the update scrolls the line on by one every 2 s
        bool                    active;
        bool                    introduction;          // what a condition of type 7 answers; entering a state with one clears it
        u8                      pad_176[0x02];
        f32                     release_delay;         // seconds until the update gives the hero back after an ending
        bool                    cancelable;            // the cancel press only ends the conversation while it's set
        bool                    unk_17d;               // set once a state has been shown since the start
        bool                    skip_line;             // the update skips the next voice line instead of playing it
        u8                      pad_17f;
        conversation*           current_conversation;
        i32                     state_stack[32];       // the states entered from the root, the shown one last
        i32                     state_depth;
        i32                     selected_reply;
        i32                     condition_results[16]; // the conversation's conditions, evaluated at the start
        u8                      lines[8][0x0C];        // eight mash::strings, the voice lines of the shown state; finalized by the destructor
        i32                     line_count;
        sound_response*         line_response;
        i32                     current_line;
        f32                     input_delay;           // seconds before the inputs are polled
        i32                     return_state;          // where an action 4 state goes back to, -1 for two states up
        i32                     line_state;            // 1 while a line plays, 2 once stopped
        i32                     unk_2c4;
        i32                     speaker_index;         // which of the conversation's speakers is the other side
        igo_3d_text*            name_text;             // "ui_HUD_conversation_name_text"
        igo_3d_text_scrollable* text;                  // "ui_HUD_conversation_text"
        igo_3d_list_box*        replies_text;          // "ui_HUD_conversation_replies_text"
        igo_3d_widget*          updown_button;         // "~updown"
        igo_3d_widget*          confirm_button;        // "~confirm"
        igo_3d_widget*          cancel_button;         // "~cancel"
        i32                     unk_2e4;               // 1 while the name shown is the other side's, 0 the hero's
        i32                     reply_count;
        u8                      reserved_2ec[0x34];

        explicit conversation_menu_system(mash::generating_vtable_version so_do_nothing) : igo_generic_input_listener(so_do_nothing),
                                                                                           igo_3d_widget(so_do_nothing) {}

        ~conversation_menu_system() override;

        // writes both source vptrs over a system the retail constructor built
        void stamp_source_vtables() {
            *(mash::vtable_value_t*)this                 = mash::generate_vtable<conversation_menu_system>();
            *(mash::vtable_value_t*)(igo_3d_widget*)this = mash::generate_vtable<conversation_menu_system, igo_3d_widget>();
        }

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void unk_002(i32 unk) override;
        void unk_003(i32 unk) override;
        void unk_004(i32 unk) override;
        void unk_005(i32 unk) override;
        void unk_006(i32 unk) override;
        void unk_007(i32 unk) override; // the confirm press
        void unk_010(i32 unk) override;

        void update(f32 time_inc) override;
        void render() override;
        void spawn_entity() override;
        i32  unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) override;
        i32  unk_047(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) override;

        bool is_conversation_active();

        // ends the running one first unless it's the same conversation
        void begin_conversation(conversation* new_conversation, bool met_before, bool can_cancel);

        void show_first_state(); // ends the conversation when no root state holds
        void show_current_state();
        void enter_state(i32 state); // nothing once 32 states deep
        void leave_state();          // ends the conversation as cancelled at the root

        // sub_68BF70
        // inlined @ sub_717520
        // inlined @ sub_7177F0
        i32 current_state() const { // -1 with no state entered
            return state_depth > 0 ? state_stack[state_depth - 1] : -1;
        }
    };

    ASSERT_SIZEOF  (conversation_menu_system,                       0x320);
    ASSERT_OFFSETOF(conversation_menu_system, unk_160,              0x160);
    ASSERT_OFFSETOF(conversation_menu_system, text_scroll_time,     0x170);
    ASSERT_OFFSETOF(conversation_menu_system, active,               0x174);
    ASSERT_OFFSETOF(conversation_menu_system, introduction,         0x175);
    ASSERT_OFFSETOF(conversation_menu_system, release_delay,        0x178);
    ASSERT_OFFSETOF(conversation_menu_system, cancelable,           0x17C);
    ASSERT_OFFSETOF(conversation_menu_system, skip_line,            0x17E);
    ASSERT_OFFSETOF(conversation_menu_system, current_conversation, 0x180);
    ASSERT_OFFSETOF(conversation_menu_system, state_stack,          0x184);
    ASSERT_OFFSETOF(conversation_menu_system, state_depth,          0x204);
    ASSERT_OFFSETOF(conversation_menu_system, selected_reply,       0x208);
    ASSERT_OFFSETOF(conversation_menu_system, condition_results,    0x20C);
    ASSERT_OFFSETOF(conversation_menu_system, lines,                0x24C);
    ASSERT_OFFSETOF(conversation_menu_system, line_count,           0x2AC);
    ASSERT_OFFSETOF(conversation_menu_system, line_response,        0x2B0);
    ASSERT_OFFSETOF(conversation_menu_system, input_delay,          0x2B8);
    ASSERT_OFFSETOF(conversation_menu_system, return_state,         0x2BC);
    ASSERT_OFFSETOF(conversation_menu_system, line_state,           0x2C0);
    ASSERT_OFFSETOF(conversation_menu_system, speaker_index,        0x2C8);
    ASSERT_OFFSETOF(conversation_menu_system, name_text,            0x2CC);
    ASSERT_OFFSETOF(conversation_menu_system, cancel_button,        0x2E0);
    ASSERT_OFFSETOF(conversation_menu_system, unk_2e4,              0x2E4);
    ASSERT_OFFSETOF(conversation_menu_system, reply_count,          0x2E8);
} // treyarch
