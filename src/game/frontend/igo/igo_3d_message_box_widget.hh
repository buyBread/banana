#pragma once

#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/color.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_message_box_widget : public igo_3d_widget {

    public:
        igo_3d_text* unk_150;

        virtual i32  unk_054(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, f32 unk_4, f32 unk_5, f32 unk_6) = 0;
        virtual void show_message(mash::string text, f32 duration, i32 unk_0, i32 unk_1, f32 unk_2, f32 unk_3, color32 message_color) = 0; // owns the string
        virtual i32  unk_056(bool unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32* unk_4, f32 unk_5, char* unk_6) = 0;
        virtual u32  unk_057(u32 unk) = 0;
        virtual i32  unk_058() = 0;
        virtual u32  unk_059() = 0;
        virtual u32  unk_060(i32 unk) = 0;

        void show_message(const char* text, f32 duration, i32 unk_0, i32 unk_1, f32 unk_2, f32 unk_3, color32 message_color) {
            show_message(mash::string(text), duration, unk_0, unk_1, unk_2, unk_3, message_color);
        }
    };

    ASSERT_OFFSETOF(igo_3d_message_box_widget, unk_150, 0x150);
} // treyarch
