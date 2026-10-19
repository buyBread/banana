#pragma once

#include "treyarch/game/light/light_source.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace ngl {
        struct texture;
    } // ngl

    struct region_horizon {
        u8            reserved_000[0x08];
        ngl::texture* texture;
    };

    struct region {
        u32             unk_000; // scene entities join the region only while it's set
        u8              reserved_004[0x70];
        u32*            renderables_begin;
        u32*            renderables_end;
        u8              reserved_07c[0x08];
        light_source**  lights_begin;
        light_source**  lights_end;
        u8              reserved_08c[0x50];
        vector3         center;
        u8              reserved_0e8[0x38];
        region_horizon* horizon;
        u8              reserved_124[0x14];
    };

    namespace references {
        // indexed by region allocation index
        inline util::memory_reference<region> region_pool { 0x01036ED8 };
    } // references

    ASSERT_OFFSETOF(region_horizon, texture, 0x08);

    ASSERT_SIZEOF  (region,                    0x138);
    ASSERT_OFFSETOF(region, renderables_begin, 0x074);
    ASSERT_OFFSETOF(region, renderables_end,   0x078);
    ASSERT_OFFSETOF(region, lights_begin,      0x084);
    ASSERT_OFFSETOF(region, lights_end,        0x088);
    ASSERT_OFFSETOF(region, center,            0x0DC);
    ASSERT_OFFSETOF(region, horizon,           0x120);
} // treyarch
