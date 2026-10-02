#pragma once

#include "treyarch/game/frontend/igo/igo_3d_widget.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // class unknown; UIFrontEnd +0x1A4 starts null and is drawn as four optional drawables
    class igo_widget_group {
        
    public:
        u32              unk_00;
        igo_3d_drawable* widgets[4];

        void draw();
    };

    ASSERT_OFFSETOF(igo_widget_group, widgets, 0x04);
} // treyarch
