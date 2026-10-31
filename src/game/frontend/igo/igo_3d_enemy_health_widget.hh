#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_enemy_health_widget;

    struct igo_3d_enemy_health_widget_vtable : igo_3d_widget_vtable {
        void* reserved_0d8;
        void  (__thiscall* set_unk_1c0)(igo_3d_enemy_health_widget* self, bool value);
        void  (__thiscall* slide_in)(igo_3d_enemy_health_widget* self);  // a timed move to the in position, then shown
        void  (__thiscall* slide_out)(igo_3d_enemy_health_widget* self); // a timed move to the out position
        void* reserved_0e8[2];
    };

    class igo_3d_enemy_health_widget : public igo_3d_widget {

    public:
        u8 reserved_150[0x70];
        u8 unk_1c0;
        u8 reserved_1c1[0x1F];

        igo_3d_enemy_health_widget_vtable* get_vtable() const {
            return (igo_3d_enemy_health_widget_vtable*)vtable;
        }

        void set_unk_1c0(bool value) {
            get_vtable()->set_unk_1c0(this, value);
        }

        void slide_in() {
            get_vtable()->slide_in(this);
        }

        void slide_out() {
            get_vtable()->slide_out(this);
        }
    };

    ASSERT_SIZEOF  (igo_3d_enemy_health_widget_vtable,              60 * 4);
    ASSERT_OFFSETOF(igo_3d_enemy_health_widget_vtable, set_unk_1c0, 0x0DC);
    ASSERT_OFFSETOF(igo_3d_enemy_health_widget_vtable, slide_in,    0x0E0);
    ASSERT_OFFSETOF(igo_3d_enemy_health_widget_vtable, slide_out,   0x0E4);

    ASSERT_SIZEOF  (igo_3d_enemy_health_widget,          0x1E0);
    ASSERT_OFFSETOF(igo_3d_enemy_health_widget, unk_1c0, 0x1C0);
} // treyarch
