#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/color.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_message_box_widget;

    // the string slot owns its by-value string
    struct igo_3d_message_box_widget_vtable : igo_3d_widget_vtable {
        void* reserved_0d8;
        void  (__thiscall* show_message)(igo_3d_message_box_widget* self,
                                         mash::string               text,
                                         f32                        duration,
                                         i32,
                                         i32,
                                         f32,
                                         f32,
                                         color32                    color);
    };

    class igo_3d_message_box_widget : public igo_3d_widget {

    public:
        igo_3d_message_box_widget_vtable* get_vtable() const {
            return (igo_3d_message_box_widget_vtable*)vtable;
        }

        void show_message(const char* text, f32 duration, i32 unk_0, i32 unk_1, f32 unk_2, f32 unk_3, color32 message_color) {
            get_vtable()->show_message(this, mash::string(text), duration, unk_0, unk_1, unk_2, unk_3, message_color);
        }

        void show_message(const mash::string &text, f32 duration, i32 unk_0, i32 unk_1, f32 unk_2, f32 unk_3, color32 message_color) {
            get_vtable()->show_message(this, mash::string(text), duration, unk_0, unk_1, unk_2, unk_3, message_color);
        }
    };

    ASSERT_OFFSETOF(igo_3d_message_box_widget_vtable, show_message, 0x0DC);
} // treyarch
