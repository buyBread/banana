#pragma once

#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class bounding_box { // L95694: SM3 .ii

    public:
        vector3 vmin;
        vector3 vmax;
    };

    ASSERT_SIZEOF  (bounding_box,       0x18);
    ASSERT_OFFSETOF(bounding_box, vmax, 0x0C);
} // treyarch
