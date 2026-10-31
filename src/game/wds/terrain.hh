#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class terrain {

    public:
        u8    reserved_000[0x04];
        void* subdivision;
        u8    reserved_008[0x04];
        i32   total_regions;
    };

    ASSERT_OFFSETOF(terrain, subdivision,   0x04);
    ASSERT_OFFSETOF(terrain, total_regions, 0x0C);
} // treyarch
