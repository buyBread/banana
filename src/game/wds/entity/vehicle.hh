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
    };

    ASSERT_OFFSETOF(vehicle, my_ai, 0x294);
} // treyarch
