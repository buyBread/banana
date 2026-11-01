#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class mission_objective_data;

    // the string slots own their by-value strings
    class igo_3d_dialog_widget : public igo_3d_widget, public igo_generic_input_listener {

    public:
        u8                      reserved_15c[0x8C];
        igo_generic_decay_timer unk_1e8;
        igo_generic_decay_timer unk_204;
        u8                      reserved_220[0x0C];
        u8                      unk_22c;
        u8                      reserved_22d[0x03];

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
        virtual void bring_up(i32 mode) = 0;
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

    ASSERT_SIZEOF  (igo_3d_dialog_widget,          0x230);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_1e8, 0x1E8);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_204, 0x204);
    ASSERT_OFFSETOF(igo_3d_dialog_widget, unk_22c, 0x22C);
} // treyarch
