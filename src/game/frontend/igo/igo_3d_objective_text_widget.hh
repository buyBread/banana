#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/message_text.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_objective_text_widget;

    // the string slot owns its by-value string
    struct igo_3d_objective_text_widget_vtable : igo_3d_widget_vtable {
        void  (__thiscall* suspend)(igo_3d_objective_text_widget* self, bool suspended);
        void  (__thiscall* show_message)(igo_3d_objective_text_widget* self, mash::string text, f32 duration);
        void* reserved_0e0[3];
    };

    class igo_3d_objective_text_widget : public igo_3d_widget {

    public:
        struct queue_entry {
            mash::string text;
            f32          unk_0c;
        };

        vc8::list<queue_entry> queue;
        message_text*          shown_message;
        igo_3d_text*           text;
        bool                   suspended;
        u8                     reserved_165[0x1B];

        igo_3d_objective_text_widget_vtable* get_vtable() const {
            return (igo_3d_objective_text_widget_vtable*)vtable;
        }

        void suspend(bool on) {
            get_vtable()->suspend(this, on);
        }

        void show_message(const char* message, f32 duration) {
            get_vtable()->show_message(this, mash::string(message), duration);
        }

        void show_message(const mash::string &message, f32 duration) {
            get_vtable()->show_message(this, mash::string(message), duration);
        }

        void clear(bool fade); // fading lets the shown message run out, either way the queue empties
    };

    class igo_3d_hint_text_widget : public igo_3d_objective_text_widget {};

    ASSERT_SIZEOF  (igo_3d_objective_text_widget_vtable,               59 * 4);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget_vtable, suspend,      0x0D8);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget_vtable, show_message, 0x0DC);

    ASSERT_SIZEOF  (igo_3d_objective_text_widget::queue_entry, 0x10);

    ASSERT_SIZEOF  (igo_3d_objective_text_widget,                0x180);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, queue,         0x150);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, shown_message, 0x15C);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, text,          0x160);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, suspended,     0x164);

    ASSERT_SIZEOF  (igo_3d_hint_text_widget, 0x180);
} // treyarch
