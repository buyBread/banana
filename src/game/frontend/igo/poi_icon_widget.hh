#pragma once

#include "game/frontend/igo/igo_3d_poi_simple_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class poi_icon_widget {

    public:
        i32                       kind;
        u32                       mode;        // set_poi_icon carries modes 1 and 2 across a kind change
        u32                       state;       // the +0x24 widget shows in states 1 and 2
        f32                       unk_0c;
        f32                       unk_10;
        u8                        reserved_014[0x09];
        bool                      is_shown;
        u8                        reserved_01e;
        u8                        unk_1f;
        bool                      dot_visible;
        u8                        reserved_021[0x03];
        igo_3d_widget*            unk_24;
        igo_3d_widget*            unk_28;      // a second life bar
        igo_3d_poi_simple_widget* unk_2c;
        u8                        reserved_030[0x0C];
        bool                      unk_3c;      // follows is_shown
        u8                        reserved_03d[0x03];

        ~poi_icon_widget();

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void draw();
    };

    ASSERT_SIZEOF  (poi_icon_widget,              0x40);
    ASSERT_OFFSETOF(poi_icon_widget, state,       0x08);
    ASSERT_OFFSETOF(poi_icon_widget, unk_10,      0x10);
    ASSERT_OFFSETOF(poi_icon_widget, is_shown,    0x1D);
    ASSERT_OFFSETOF(poi_icon_widget, unk_1f,      0x1F);
    ASSERT_OFFSETOF(poi_icon_widget, dot_visible, 0x20);
    ASSERT_OFFSETOF(poi_icon_widget, unk_24,      0x24);
    ASSERT_OFFSETOF(poi_icon_widget, unk_2c,      0x2C);
    ASSERT_OFFSETOF(poi_icon_widget, unk_3c,      0x3C);
} // treyarch
