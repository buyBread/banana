#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_enemy_health_manager {
        
    public:
        void*            vtable;
        igo_3d_drawable* unk_04;
        igo_3d_drawable* unk_08;

        void draw();
    };

    ASSERT_OFFSETOF(igo_3d_enemy_health_manager, unk_08, 0x08);
} // treyarch
