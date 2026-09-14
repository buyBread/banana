#pragma once

#include "treyarch/game/light/light_source.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct region {
        u8             reserved_000[0x74];
        u32*           renderables_begin;
        u32*           renderables_end;
        u8             reserved_07c[0x08];
        light_source** lights_begin;
        light_source** lights_end;
    };

    ASSERT_SIZEOF  (region,                    0x8C);
    ASSERT_OFFSETOF(region, renderables_begin, 0x74);
    ASSERT_OFFSETOF(region, renderables_end,   0x78);
    ASSERT_OFFSETOF(region, lights_begin,      0x84);
    ASSERT_OFFSETOF(region, lights_end,        0x88);
} // treyarch
