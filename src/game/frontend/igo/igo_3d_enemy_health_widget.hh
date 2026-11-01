#pragma once

#include "game/frontend/igo/igo_3d_slideable_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_icon_health_widget : public igo_3d_slideable_widget {

    public:
        virtual f32 unk_054(f32 unk_0, f32 unk_1, f32 unk_2) = 0;
    };

    class igo_3d_enemy_health_widget : public igo_3d_icon_health_widget {

    public:
        u8 reserved_150[0x70];
        u8 unk_1c0;
        u8 reserved_1c1[0x1F];

        virtual void set_unk_1c0(bool value) = 0;
        virtual void slide_in() = 0;  // a timed move to the in position, then shown
        virtual void slide_out() = 0; // a timed move to the out position
        virtual i32  unk_058() = 0;
        virtual i32  unk_059(i32 unk_0, i32 unk_1, f32* unk_2, i32 unk_3, i32 unk_4, void* unk_5, bool unk_6) = 0;
    };

    ASSERT_SIZEOF  (igo_3d_enemy_health_widget,          0x1E0);
    ASSERT_OFFSETOF(igo_3d_enemy_health_widget, unk_1c0, 0x1C0);
} // treyarch
