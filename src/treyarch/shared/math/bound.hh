#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class bounding_box {

    public:
        vector3d vmin;
        vector3d vmax;
    };

    ASSERT_SIZEOF  (bounding_box,       0x18);
    ASSERT_OFFSETOF(bounding_box, vmax, 0x0C);
} // treyarch
