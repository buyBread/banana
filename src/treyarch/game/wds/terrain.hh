#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class terrain {

    public:
        u8    reserved_000[0x04];
        void* subdivision;
        u8    reserved_008[0x04];
        u32   generation;
    };

    ASSERT_OFFSETOF(terrain, subdivision, 0x04);
    ASSERT_OFFSETOF(terrain, generation,  0x0C);
} // treyarch
