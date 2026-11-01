#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "game/frontend/igo/simple_text_nav_button.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_text;
    class mission_objective_data;

    namespace references {
        inline util::memory_reference<string_hash> dialog_3d_entered   { 0x0102CAA8 };
        inline util::memory_reference<string_hash> dialog_3d_selection { 0x0102C52C };
    } // references

    // the string slots own their by-value strings
    class igo_3d_dialog_widget : public igo_3d_widget, public igo_generic_input_listener {

    public:
        bool                      game_was_paused;    // bring_up pauses the game unless it was
        u8                        reserved_15d[0x02];
        bool                      button_1_shown;
        bool                      button_2_shown;
        u8                        pad_161[0x03];
        i32                       bring_up_mode;
        u8                        reserved_168[0x10];
        vc8::vector<igo_3d_text*> lines;
        u8                        reserved_188[0x3C];
        i32                       first_visible_line; // shows five lines from here
        simple_text_nav_button*   button_1;
        simple_text_nav_button*   button_2;
        u8                        reserved_1d0[0x18];
        igo_generic_decay_timer   unk_1e8;
        igo_generic_decay_timer   unk_204;
        u8                        reserved_220[0x0C];
        u8                        unk_22c;
        u8                        reserved_22d[0x03];

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void set_title_by_id(u32 text_id) = 0;
        virtual void set_title(mash::string title) = 0;
        virtual void set_text_by_id(u32 text_id) = 0;
        virtual void set_text(mash::string text) = 0;
        virtual void clear() = 0;
        virtual void add_objective(mission_objective_data* objective, bool indented) = 0;
        virtual void add_mission_objective(mash::string line, i32 points, f32 unk_0, f32 unk_1, bool unk_2) = 0;
        virtual void add_text_line(mash::string line) = 0;
        virtual i32  unk_062(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5) = 0;
        virtual i32  unk_063(bool unk_0, i32 unk_1, i32 unk_2, char* unk_3) = 0;
        virtual void bring_up(i32 mode) = 0; // has a body; called directly, never through the slot
        virtual void close() = 0;
        virtual bool unk_066() = 0;
        virtual bool unk_067() = 0;
        virtual bool unk_068() = 0;
        virtual u8   is_dialog_shown() = 0;
        virtual bool unk_070() = 0;
        virtual bool unk_071(bool unk) = 0;
        virtual i32  get_selected_button() = 0; // 1-based while a button is shown
        virtual i32  unk_073() = 0;
        virtual i32  unk_074() = 0;
        virtual i32  unk_075(i32 unk) = 0;
        virtual bool unk_076(i32 unk) = 0;
        virtual void unk_077(i32 unk) = 0;
        virtual void unk_078() = 0;

        bool set_first_visible_line(i32 index); // clamped to the lines, true when it changed

        void set_title(const char* title) {
            set_title(mash::string(title));
        }

        void set_text(const char* text) {
            set_text(mash::string(text));
        }

        void add_text_line(const char* line) { // add_mission_objective with no points
            add_text_line(mash::string(line));
        }
    };

    ASSERT_SIZEOF  (igo_3d_dialog_widget,                     0x230);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, game_was_paused,    0x15C);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_1_shown,     0x15F);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_2_shown,     0x160);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, bring_up_mode,      0x164);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, lines,              0x178);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, first_visible_line, 0x1C4);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_1,           0x1C8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, button_2,           0x1CC);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_1e8,            0x1E8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_204,            0x204);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_22c,            0x22C);
} // treyarch
