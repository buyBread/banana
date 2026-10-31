#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_script_widget;

    struct igo_3d_script_widget_vtable : igo_3d_widget_vtable {
        void* reserved_0d8;
        void  (__thiscall* set_continuous_rotation)(igo_3d_script_widget* self, f32 rate);
        void  (__thiscall* set_continuous_tilt)(igo_3d_script_widget* self, f32 rate);
        void  (__thiscall* set_continuous_spin)(igo_3d_script_widget* self, f32 rate);
        void* reserved_0e8[2];
        u8    (__thiscall* unk_0f0)(igo_3d_script_widget* self);         // returns unk_168
        u32   (__thiscall* get_instance_id)(igo_3d_script_widget* self);
    };

    class igo_3d_script_widget : public igo_3d_widget {
    public:
        f32                     continuous_rotation;
        f32                     continuous_tilt;
        f32                     continuous_spin;
        vc8::list<igo_3d_text*> texts;
        u8                      unk_168;
        u8                      pad_169[0x03];
        f32                     max_timer;
        u8                      reserved_170[0x08];
        string_hash             text_parameters_key; // every text is built with it
        u8                      unk_17c;
        bool                    persistent;          // drawn while the interface is disabled
        u8                      pad_17e[0x02];
        u32                     instance_id;
        u8                      reserved_184[0x0C];

        igo_3d_script_widget_vtable* get_vtable() const {
            return (igo_3d_script_widget_vtable*)vtable;
        }

        void set_continuous_rotation(f32 rate) {
            get_vtable()->set_continuous_rotation(this, rate);
        }

        void set_continuous_tilt(f32 rate) {
            get_vtable()->set_continuous_tilt(this, rate);
        }

        void set_continuous_spin(f32 rate) {
            get_vtable()->set_continuous_spin(this, rate);
        }

        u8 unk_0f0() {
            return get_vtable()->unk_0f0(this);
        }

        u32 get_instance_id() {
            return get_vtable()->get_instance_id(this);
        }

        igo_3d_text* get_text(i32 id); // made on first use; -2 answers null
    };

    ASSERT_SIZEOF  (igo_3d_script_widget_vtable,                          62 * 4);
    ASSERT_OFFSETOF(igo_3d_script_widget_vtable, set_continuous_rotation, 0x0DC);
    ASSERT_OFFSETOF(igo_3d_script_widget_vtable, unk_0f0,                 0x0F0);
    ASSERT_OFFSETOF(igo_3d_script_widget_vtable, get_instance_id,         0x0F4);

    ASSERT_SIZEOF  (igo_3d_script_widget,                      0x190);
    ASSERT_OFFSETOF(igo_3d_script_widget, continuous_rotation, 0x150);
    ASSERT_OFFSETOF(igo_3d_script_widget, texts,               0x15C);
    ASSERT_OFFSETOF(igo_3d_script_widget, unk_168,             0x168);
    ASSERT_OFFSETOF(igo_3d_script_widget, max_timer,           0x16C);
    ASSERT_OFFSETOF(igo_3d_script_widget, text_parameters_key, 0x178);
    ASSERT_OFFSETOF(igo_3d_script_widget, persistent,          0x17D);
    ASSERT_OFFSETOF(igo_3d_script_widget, instance_id,         0x180);
} // treyarch
