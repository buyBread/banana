#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_pauseless_dialog_widget;
    class mission_objective_data;

    // the string slots own their by-value strings
    struct igo_3d_pauseless_dialog_widget_vtable : igo_3d_widget_vtable {
        void  (__thiscall* set_title_by_id)(igo_3d_pauseless_dialog_widget* self, u32 text_id);
        void  (__thiscall* set_title)(igo_3d_pauseless_dialog_widget* self, mash::string title);
        void  (__thiscall* set_text_by_id)(igo_3d_pauseless_dialog_widget* self, u32 text_id);
        void  (__thiscall* set_text)(igo_3d_pauseless_dialog_widget* self, mash::string text);
        void  (__thiscall* clear)(igo_3d_pauseless_dialog_widget* self);
        void  (__thiscall* add_objective)(igo_3d_pauseless_dialog_widget* self, mission_objective_data* objective, bool indented);
        void  (__thiscall* add_mission_objective)(igo_3d_pauseless_dialog_widget* self, mash::string line, i32 points, f32, f32, bool);
        void  (__thiscall* add_text_line)(igo_3d_pauseless_dialog_widget* self, mash::string line); // add_mission_objective with no points
        void* reserved_0f8[2];
        void  (__thiscall* bring_up)(igo_3d_pauseless_dialog_widget* self, i32 mode);
        void  (__thiscall* close)(igo_3d_pauseless_dialog_widget* self);
        void* reserved_108[3];
        u8    (__thiscall* is_dialog_shown)(igo_3d_pauseless_dialog_widget* self);
        void* reserved_118[2];
        i32   (__thiscall* get_selected_button)(igo_3d_pauseless_dialog_widget* self);
        void* reserved_124[6];
    };

    class igo_3d_pauseless_dialog_widget : public igo_3d_widget {

    public:
        u8                      reserved_150[0x98];
        igo_generic_decay_timer unk_1e8;
        igo_generic_decay_timer unk_204;
        u8                      reserved_220[0x0C];
        u8                      unk_22c;
        u8                      reserved_22d[0x23];

        igo_3d_pauseless_dialog_widget_vtable* get_vtable() const {
            return (igo_3d_pauseless_dialog_widget_vtable*)vtable;
        }

        void set_title_by_id(u32 text_id) {
            get_vtable()->set_title_by_id(this, text_id);
        }

        void set_title(const char* title) {
            get_vtable()->set_title(this, mash::string(title));
        }

        void set_title(const mash::string &title) {
            get_vtable()->set_title(this, mash::string(title));
        }

        void set_text(const char* text) {
            get_vtable()->set_text(this, mash::string(text));
        }

        void set_text(const mash::string &text) {
            get_vtable()->set_text(this, mash::string(text));
        }

        void clear() {
            get_vtable()->clear(this);
        }

        void add_text_line(const char* line) {
            get_vtable()->add_text_line(this, mash::string(line));
        }

        void close() {
            get_vtable()->close(this);
        }

        u8 is_dialog_shown() {
            return get_vtable()->is_dialog_shown(this);
        }

        i32 get_selected_button() { // 1-based while a button is shown
            return get_vtable()->get_selected_button(this);
        }
    };

    ASSERT_SIZEOF  (igo_3d_pauseless_dialog_widget_vtable,                      79 * 4);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, set_title_by_id,     0x0D8);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, set_text,            0x0E4);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, clear,               0x0E8);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, add_text_line,       0x0F4);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, bring_up,            0x100);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, close,               0x104);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, is_dialog_shown,     0x114);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget_vtable, get_selected_button, 0x120);

    ASSERT_SIZEOF  (igo_3d_pauseless_dialog_widget,          0x250);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_1e8, 0x1E8);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_204, 0x204);
    ASSERT_OFFSETOF(igo_3d_pauseless_dialog_widget, unk_22c, 0x22C);
} // treyarch
