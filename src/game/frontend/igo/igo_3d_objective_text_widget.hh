#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/message_text.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
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

        virtual void suspend(bool on) = 0;
        virtual void show_message(mash::string text, f32 duration) = 0; // owns the string
        virtual i32  unk_056(i32 unk_0, f32 unk_1) = 0;
        virtual void unk_057(i32 unk) = 0;
        virtual bool unk_058() = 0;

        void show_message(const char* message, f32 duration) {
            show_message(mash::string(message), duration);
        }

        void clear(bool fade); // fading lets the shown message run out, either way the queue empties
    };

    class igo_3d_hint_text_widget : public igo_3d_objective_text_widget {

    public:
        virtual i32 unk_059(bool unk_0, i32 unk_1, i32 unk_2, i32 unk_3, f32 unk_4) = 0;
        virtual i32 unk_060(i32 unk_0, i32 unk_1, f32 unk_2) = 0;
    };

    ASSERT_SIZEOF  (igo_3d_objective_text_widget::queue_entry, 0x10);

    ASSERT_SIZEOF  (igo_3d_objective_text_widget,                0x180);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, queue,         0x150);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, shown_message, 0x15C);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, text,          0x160);
    ASSERT_OFFSETOF(igo_3d_objective_text_widget, suspended,     0x164);

    ASSERT_SIZEOF  (igo_3d_hint_text_widget, 0x180);
} // treyarch
