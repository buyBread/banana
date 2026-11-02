#pragma once

#include "game/frontend/igo/igo_3d_poi_simple_widget.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a second life bar
    class poi_icon_widget_unk_28 : public igo_3d_widget {

    public:
        u8  reserved_150[0x30];
        i32 life_bar_health;
        i32 unk_184;
    };

    class poi_icon_widget {

    public:
        i32                       kind;
        u32                       mode;        // set_poi_icon carries modes 1 and 2 across a kind change
        u32                       state;       // the +0x24 widget shows in states 1 and 2
        f32                       unk_0c;
        f32                       unk_10;
        u8                        reserved_014[0x04];
        f32                       unk_18;
        u8                        unk_1c;
        bool                      is_shown;
        u8                        unk_1e;
        u8                        unk_1f;
        bool                      dot_visible;
        u8                        reserved_021[0x03];
        igo_3d_widget*            unk_24;
        poi_icon_widget_unk_28*   unk_28;
        igo_3d_poi_simple_widget* unk_2c;
        f32                       unk_30;      // the +0x2C widget's scale
        f32                       unk_34;
        f32                       unk_38;
        bool                      unk_3c;      // follows is_shown
        u8                        reserved_03d[0x03];

        explicit poi_icon_widget(i32 kind);
        ~poi_icon_widget();

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        static i32 widget_style(i32 kind); // the +0x2C widget's style

        void turn_on(bool on);
        void draw();

        void set_life_bar_values(i32 health, i32 unk);
        i32  get_life_bar_health();
    };

    ASSERT_OFFSETOF(poi_icon_widget_unk_28, life_bar_health, 0x180);
    ASSERT_OFFSETOF(poi_icon_widget_unk_28, unk_184,         0x184);

    ASSERT_SIZEOF  (poi_icon_widget,              0x40);
    ASSERT_OFFSETOF(poi_icon_widget, state,       0x08);
    ASSERT_OFFSETOF(poi_icon_widget, unk_10,      0x10);
    ASSERT_OFFSETOF(poi_icon_widget, unk_18,      0x18);
    ASSERT_OFFSETOF(poi_icon_widget, is_shown,    0x1D);
    ASSERT_OFFSETOF(poi_icon_widget, unk_1f,      0x1F);
    ASSERT_OFFSETOF(poi_icon_widget, dot_visible, 0x20);
    ASSERT_OFFSETOF(poi_icon_widget, unk_24,      0x24);
    ASSERT_OFFSETOF(poi_icon_widget, unk_2c,      0x2C);
    ASSERT_OFFSETOF(poi_icon_widget, unk_30,      0x30);
    ASSERT_OFFSETOF(poi_icon_widget, unk_3c,      0x3C);
} // treyarch
