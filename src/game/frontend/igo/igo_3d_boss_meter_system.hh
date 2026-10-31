#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_boss_meter_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_boss_meter_system {

    public:
        void*                           vtable;
        igo_3d_widget*                  default_meter; // the "boss_meter" widget the meters line up under
        vc8::list
            <igo_3d_boss_meter_widget*> meters;
        i32                             last_meter_id;

        void draw();

        void remove_all_meters();
        void remove_meter(i32 id); // every meter with the id
        void position_meters();    // stacks the meters 60 apart from the default meter's position

        f32  get_health(i32 id);   // -1 without a meter of that id
        void set_health(i32 id, f32 health);
    };

    ASSERT_SIZEOF  (igo_3d_boss_meter_system,                0x18);
    ASSERT_OFFSETOF(igo_3d_boss_meter_system, default_meter, 0x04);
    ASSERT_OFFSETOF(igo_3d_boss_meter_system, meters,        0x08);
    ASSERT_OFFSETOF(igo_3d_boss_meter_system, last_meter_id, 0x14);
} // treyarch
