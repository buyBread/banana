#pragma once

#include "treyarch/shared/math/po.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    enum light_flavor : u32 {
        light_flavor_point       = 0,
        light_flavor_spot        = 1,
        light_flavor_directional = 2
    };

    struct light_properties {
        light_flavor flavor;
        vector4      diffuse_color;
        u8           reserved_014[0x1C];
        f32          inner_angle;
        f32          outer_angle;
    };

    class light_source {

    public:
        void**            vtable;
        u8                reserved_004[0x0C];
        po*               my_abs_po;
        u8                reserved_014[0x64];
        u32               render_generations[10];
        light_properties* properties;
    };

    ASSERT_SIZEOF  (light_properties,             0x38);
    ASSERT_OFFSETOF(light_properties, flavor,      0x00);
    ASSERT_OFFSETOF(light_properties, diffuse_color, 0x04);
    ASSERT_OFFSETOF(light_properties, inner_angle, 0x30);
    ASSERT_OFFSETOF(light_properties, outer_angle, 0x34);

    ASSERT_SIZEOF  (light_source,                    0xA4);
    ASSERT_OFFSETOF(light_source, my_abs_po,           0x10);
    ASSERT_OFFSETOF(light_source, render_generations, 0x78);
    ASSERT_OFFSETOF(light_source, properties,         0xA0);
} // treyarch
