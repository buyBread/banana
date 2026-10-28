#pragma once

#include "game/color.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a radar icon
    class igo_map_poi {

    public:
        vector3d position;
        color32  color;
        i32      kind;
        u32      unk_14;            // handed to the radar when it draws city-life icons
        bool     is_shown;
        u8       unk_19[0x05];
        u8       reserved_01e[0x02];
        f32      unk_20;
        f32      unk_24;
        u8       unk_28[0x06];
        u8       reserved_02e[0x02];
        f32      unk_30;
        f32      unk_34;
        f32      unk_38;            // zeroed whenever set_poi_active changes unk_3c
        bool     unk_3c;
        u8       reserved_03d[0x03];
        f32      unk_40;
        u32      unk_44;
        u32      unk_48;
        u8       unk_4c;
        u8       reserved_04d[0x03];

        igo_map_poi(i32 kind, u32 unk_14, vector3d position);

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void set_color(color32 new_color);
    };

    ASSERT_SIZEOF  (igo_map_poi,           0x50);
    ASSERT_OFFSETOF(igo_map_poi, color,    0x0C);
    ASSERT_OFFSETOF(igo_map_poi, kind,     0x10);
    ASSERT_OFFSETOF(igo_map_poi, is_shown, 0x18);
    ASSERT_OFFSETOF(igo_map_poi, unk_20,   0x20);
    ASSERT_OFFSETOF(igo_map_poi, unk_28,   0x28);
    ASSERT_OFFSETOF(igo_map_poi, unk_30,   0x30);
    ASSERT_OFFSETOF(igo_map_poi, unk_3c,   0x3C);
    ASSERT_OFFSETOF(igo_map_poi, unk_40,   0x40);
    ASSERT_OFFSETOF(igo_map_poi, unk_4c,   0x4C);
} // treyarch
