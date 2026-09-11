#pragma once

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class po { // dragon warrior?!

public:
        matrix4x4 matrix;

        const vector3 &get_position() const {
            return matrix.w_row();
        }
    };
} // treyarch
