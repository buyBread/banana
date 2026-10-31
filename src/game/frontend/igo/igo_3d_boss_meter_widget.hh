#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_generic_decay_timer.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_boss_meter_widget : public igo_3d_widget {

    public:
        u32                     unk_150;
        i32                     id;      // handed out by the boss meter system
        i32                     unk_158;
        f32                     health;
        f32                     unk_160;
        u8                      unk_164; // set, the system doesn't line it up with the others
        u8                      unk_165; // set_health only stores while it's set
        u8                      pad_166[0x02];
        i32                     unk_168;
        arch_base_vhandle       unk_16c; // the entity the meter was added for
        igo_generic_decay_timer unk_170;
        u8                      pad_18c[0x04];
    };

    ASSERT_SIZEOF  (igo_3d_boss_meter_widget,          0x190);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, id,      0x154);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, health,  0x15C);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, unk_164, 0x164);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, unk_165, 0x165);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, unk_16c, 0x16C);
    ASSERT_OFFSETOF(igo_3d_boss_meter_widget, unk_170, 0x170);
} // treyarch
