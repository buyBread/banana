#pragma once

#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class camera {

    public:
        u8  reserved_000[0x10];
        po* my_abs_po;

        const vector3 &get_abs_position() const {
            return my_abs_po->get_position();
        }
    };

    using camera_handle = camera*;

    ASSERT_OFFSETOF(camera, my_abs_po, 0x10);
} // treyarch
