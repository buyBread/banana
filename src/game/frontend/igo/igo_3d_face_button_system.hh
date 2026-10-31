#pragma once

#include <array>

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_face_button_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_input_listener.hh"
#include "game/math/algebra.hh"
#include "game/mutex.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        // the anims of the 12 button kinds, 23 slots each; filled by a crt initializer (sub_B422C0)
        inline util::memory_reference<std::array<std::array<tl_fixed_string, 23>, 12>> button_anim_names { 0x0102D040 };

        // the widget records of the 12 button kinds, "ui_button_xe_a" first; hashed by a crt initializer (sub_B42170)
        inline util::memory_reference<std::array<string_hash, 12>> button_widget_keys { 0x0102CF64 };
    } // references

    class igo_3d_face_button_system;

    struct igo_3d_face_button_system_vtable : igo_generic_input_listener_vtable {
        void* reserved_060[3];
        void  (__thiscall* draw_buttons)(igo_3d_face_button_system* self);
    };

    class igo_3d_face_button_system : public igo_generic_input_listener {

    public:
        igo_3d_widget*                   default_button;    // the "face_button_default" widget new buttons copy
        vc8::list
            <igo_3d_face_button_widget*> buttons;
        u8                               reserved_1c[0x04];
        engine_recursive_lock            lock;              // only the removals take it
        volatile i32                     last_button_id;
        u8                               reserved_34[0x04];

        igo_3d_face_button_system_vtable* get_vtable() const {
            return (igo_3d_face_button_system_vtable*)vtable;
        }

        void draw_buttons() {
            get_vtable()->draw_buttons(this);
        }

        // inlined @ sub_6B8860
        // inlined @ sub_6B8B40
        // inlined @ sub_6B8BA0
        // inlined @ sub_6D07B0
        // inlined @ sub_6D0810
        // inlined @ sub_6D08D0
        // inlined @ sub_6D0910
        igo_3d_face_button_widget* find_button(i32 id) const { // the first listed button with the id
            for (auto* position = buttons.begin(); position != buttons.end(); position = position->next) {
                if (position->value && position->value->id == id)
                    return position->value;
            }

            return nullptr;
        }

        void remove_all_buttons();
        void remove_button(i32 id); // every button with the id

        bool is_button_anim_playing(i32 id); // false without a button of that id

        void set_button_pos  (i32 id, f32 x, f32 y);
        void set_button_pos  (i32 id, vector3d pos); // z through set_position_z
        void set_button_scale(i32 id, f32 scale);
        void set_button_alpha(i32 id, f32 alpha);

        void show_x_over_button    (i32 id, const bool &shown);
        bool is_x_over_button_shown(i32 id);

        string_hash   get_button_widget_key(u32 kind);                                  // the "" hash past the table
        mash::string* get_button_anim_name (mash::string* result, u32 kind, u32 index); // builds into result; "" past the table
    };

    ASSERT_SIZEOF  (igo_3d_face_button_system_vtable,               28 * 4);
    ASSERT_OFFSETOF(igo_3d_face_button_system_vtable, draw_buttons, 0x06C);

    ASSERT_SIZEOF  (igo_3d_face_button_system,                 0x38);
    ASSERT_OFFSETOF(igo_3d_face_button_system, default_button, 0x0C);
    ASSERT_OFFSETOF(igo_3d_face_button_system, buttons,        0x10);
    ASSERT_OFFSETOF(igo_3d_face_button_system, lock,           0x20);
    ASSERT_OFFSETOF(igo_3d_face_button_system, last_button_id, 0x30);
} // treyarch
