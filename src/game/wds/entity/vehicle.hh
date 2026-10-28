#pragma once

#include "game/wds/entity/conglomerate.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_vehicle;

    class vehicle : public conglomerate {

    public:
        u8          reserved_198[0xFC];
        ai_vehicle* my_ai;
        u8          reserved_298[0x48];
        i32         condition;
    };

    ASSERT_OFFSETOF(vehicle, my_ai,     0x294);
    ASSERT_OFFSETOF(vehicle, condition, 0x2E0);
} // treyarch
