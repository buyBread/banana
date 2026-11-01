#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_face_button_widget : public igo_3d_widget {

    public:
        u8             reserved_150[0x04];
        u32            kind;
        i32            id;         // handed out by the face button system
        u8             reserved_15c[0x20];
        vector2d       button_pos; // slot 57 stores it
        u8             reserved_184[0x08];
        igo_3d_widget* web_widget;
        igo_3d_widget* x_widget;
        u8             reserved_194[0x1C];

        virtual bool  unk_054(f32 unk_0, f32 unk_1) = 0;
        virtual bool  unk_055() = 0;
        virtual bool  unk_056() = 0;
        virtual void  set_button_pos(f32 x, f32 y) = 0;
        virtual void* unk_058(void* unk) = 0;
        virtual bool  unk_059(i32 unk_0, i32 unk_1) = 0;
        virtual bool  unk_060() = 0;
        virtual i32   unk_061(u8* unk) = 0;
        virtual bool  unk_062() = 0;
        virtual i32   unk_063(i32 unk) = 0;
        virtual void  unk_064() = 0;
        virtual void  unk_065() = 0;
        virtual bool  unk_066() = 0;
        virtual i32   unk_067(u8* unk) = 0;
        virtual bool  unk_068() = 0;
        virtual void  unk_069() = 0;
        virtual void  init_x_widget() = 0;
        virtual u8    has_x_widget() = 0; // its body tests the web widget
        virtual i32   unk_072(u8* unk) = 0;
        virtual u8    is_x_widget_shown() = 0;
        virtual void  destroy_x_widget() = 0;
    };

    ASSERT_SIZEOF  (igo_3d_face_button_widget,             0x1B0);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, kind,       0x154);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, id,         0x158);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, button_pos, 0x17C);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, web_widget, 0x18C);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, x_widget,   0x190);
} // treyarch
