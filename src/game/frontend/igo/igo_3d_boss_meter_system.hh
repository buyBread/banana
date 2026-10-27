#pragma once

#include "vc8/list.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_boss_meter_system {
        
    public:
        void*                  vtable;
        u32                    unk_04;
        vc8::list
            <igo_3d_drawable*> meters;

        void draw();
    };

    ASSERT_OFFSETOF(igo_3d_boss_meter_system, meters, 0x08);
} // treyarch
