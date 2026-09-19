#pragma once

#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class entity {

    public:
        void** vtable;
        u8     reserved_004[0x0C];
        po*    my_abs_po;
        u8     reserved_014[0x08];
        u32*   render_object_table;
    };

    ASSERT_OFFSETOF(entity, my_abs_po,           0x10);
    ASSERT_OFFSETOF(entity, render_object_table, 0x1C);
} // treyarch
