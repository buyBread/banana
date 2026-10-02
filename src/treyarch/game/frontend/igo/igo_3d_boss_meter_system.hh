#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_boss_meter_system {
        
    public:
        void*                  vtable;
        u32                    unk_04;
        dinkumware::list
            <igo_3d_drawable*> meters;

        void draw();
    };

    ASSERT_OFFSETOF(igo_3d_boss_meter_system, meters, 0x08);
} // treyarch
