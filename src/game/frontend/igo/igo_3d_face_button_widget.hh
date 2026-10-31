#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_face_button_widget;

    struct igo_3d_face_button_widget_vtable : igo_3d_widget_vtable {
        void* reserved_0d8[3];
        void  (__thiscall* set_button_pos)(igo_3d_face_button_widget* self, f32 x, f32 y);
        void* reserved_0e8[12];
        void  (__thiscall* init_x_widget)(igo_3d_face_button_widget* self);
        u8    (__thiscall* has_x_widget)(igo_3d_face_button_widget* self); // its body tests the web widget
        void* reserved_120;
        u8    (__thiscall* is_x_widget_shown)(igo_3d_face_button_widget* self);
        void  (__thiscall* destroy_x_widget)(igo_3d_face_button_widget* self);
    };

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

        igo_3d_face_button_widget_vtable* get_vtable() const {
            return (igo_3d_face_button_widget_vtable*)vtable;
        }

        void set_button_pos(f32 x, f32 y) {
            get_vtable()->set_button_pos(this, x, y);
        }

        void init_x_widget() {
            get_vtable()->init_x_widget(this);
        }

        u8 has_x_widget() {
            return get_vtable()->has_x_widget(this);
        }

        u8 is_x_widget_shown() {
            return get_vtable()->is_x_widget_shown(this);
        }

        void destroy_x_widget() {
            get_vtable()->destroy_x_widget(this);
        }
    };

    ASSERT_SIZEOF  (igo_3d_face_button_widget_vtable,                    75 * 4);
    ASSERT_OFFSETOF(igo_3d_face_button_widget_vtable, set_button_pos,    0x0E4);
    ASSERT_OFFSETOF(igo_3d_face_button_widget_vtable, init_x_widget,     0x118);
    ASSERT_OFFSETOF(igo_3d_face_button_widget_vtable, has_x_widget,      0x11C);
    ASSERT_OFFSETOF(igo_3d_face_button_widget_vtable, is_x_widget_shown, 0x124);
    ASSERT_OFFSETOF(igo_3d_face_button_widget_vtable, destroy_x_widget,  0x128);

    ASSERT_SIZEOF  (igo_3d_face_button_widget,             0x1B0);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, kind,       0x154);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, id,         0x158);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, button_pos, 0x17C);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, web_widget, 0x18C);
    ASSERT_OFFSETOF(igo_3d_face_button_widget, x_widget,   0x190);
} // treyarch
