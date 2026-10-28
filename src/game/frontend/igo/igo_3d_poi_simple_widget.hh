#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a poi icon's distance text and life bar
    class igo_3d_poi_simple_widget : public igo_3d_widget {

    public:
        u8   reserved_150[0x10];
        i32  life_bar_health;
        i32  unk_164;
        u8   reserved_168[0x2D];
        bool is_life_bar;
        u8   reserved_196[0x2A];
    };

    ASSERT_SIZEOF  (igo_3d_poi_simple_widget,                  0x1C0);
    ASSERT_OFFSETOF(igo_3d_poi_simple_widget, life_bar_health, 0x160);
    ASSERT_OFFSETOF(igo_3d_poi_simple_widget, unk_164,         0x164);
    ASSERT_OFFSETOF(igo_3d_poi_simple_widget, is_life_bar,     0x195);
} // treyarch
