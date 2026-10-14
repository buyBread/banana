#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct color {
        f32 r, g, b, a;
    };

    ASSERT_SIZEOF(color, 0x10);
} // treyarch
