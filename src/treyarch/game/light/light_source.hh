#pragma once

#include <cstddef>

#include "treyarch/shared/color.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/po.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace ngl {
        struct texture;
    } // ngl

    enum light_flavor : u32 {
        light_flavor_point       = 0,
        light_flavor_spot        = 1,
        light_flavor_directional = 2
    };

    struct light_properties {
        light_flavor  flavor;
        color         diffuse_color;
        color         additive_color;
        f32           near_range;
        f32           cutoff_range;
        f32           recip_cutoff_minus_near;
        f32           inner_angle;
        f32           outer_angle;
        u32           lightcat;
        u32           flags;
        ngl::texture* tex;
        vector3d      scale;

        light_properties(light_flavor flavor,
                         color        diffuse_color,
                         color        additive_color,
                         f32          near_range,
                         f32          cutoff_range);
    };

    class light_source {

    public:
        void**            vtable;
        u8                reserved_004[0x0C];
        po*               my_abs_po;
        u8                reserved_014[0x64];
        u32               render_generations[10];
        light_properties* properties;

        light_source(const light_properties &properties, const po* the_po, const string_hash &id);

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    ASSERT_SIZEOF  (light_properties,                0x50);
    ASSERT_OFFSETOF(light_properties, flavor,        0x00);
    ASSERT_OFFSETOF(light_properties, diffuse_color, 0x04);
    ASSERT_OFFSETOF(light_properties, near_range,    0x24);
    ASSERT_OFFSETOF(light_properties, inner_angle,   0x30);
    ASSERT_OFFSETOF(light_properties, outer_angle,   0x34);
    ASSERT_OFFSETOF(light_properties, lightcat,      0x38);
    ASSERT_OFFSETOF(light_properties, scale,         0x44);

    ASSERT_SIZEOF  (light_source,                     0xA4);
    ASSERT_OFFSETOF(light_source, my_abs_po,          0x10);
    ASSERT_OFFSETOF(light_source, render_generations, 0x78);
    ASSERT_OFFSETOF(light_source, properties,         0xA0);
} // treyarch
