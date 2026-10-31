#pragma once

#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_generic_meter_widget;

    struct igo_3d_generic_meter_widget_vtable : igo_3d_widget_vtable {
        void  (__thiscall* set_value)(igo_3d_generic_meter_widget* self, f32 value);
        void* reserved_0dc[2];
    };

    class igo_3d_generic_meter_widget : public igo_3d_widget {

    public:
        i32          unk_150;
        i32          unk_154;
        f32          value;
        f32          unk_15c;
        u8           unk_160;
        u8           unk_161;
        u8           pad_162[0x02];
        igo_3d_text* unk_164; // the label lands on one of these two
        igo_3d_text* unk_168;
        u8           reserved_16c[0x14];

        igo_3d_generic_meter_widget_vtable* get_vtable() const {
            return (igo_3d_generic_meter_widget_vtable*)vtable;
        }

        void set_value(f32 new_value) {
            get_vtable()->set_value(this, new_value);
        }
    };

    ASSERT_SIZEOF  (igo_3d_generic_meter_widget_vtable,            57 * 4);
    ASSERT_OFFSETOF(igo_3d_generic_meter_widget_vtable, set_value, 0x0D8);

    ASSERT_SIZEOF  (igo_3d_generic_meter_widget,          0x180);
    ASSERT_OFFSETOF(igo_3d_generic_meter_widget, value,   0x158);
    ASSERT_OFFSETOF(igo_3d_generic_meter_widget, unk_164, 0x164);
    ASSERT_OFFSETOF(igo_3d_generic_meter_widget, unk_168, 0x168);
} // treyarch
