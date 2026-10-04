#pragma once

#include "treyarch/game/wds/entity/entity.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class ai_core;

    struct base_ai_data {
        u8       reserved_000[0x28];
        ai_core* core;
    };

    class actor : public entity {

    public:
        u8            reserved_074[0x5C];
        base_ai_data* my_base_ai_data;

        ai_core* get_ai_core() const;
    };

    ASSERT_OFFSETOF(base_ai_data, core, 0x28);

    ASSERT_OFFSETOF(actor, my_base_ai_data, 0xD0);
} // treyarch
