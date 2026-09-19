#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class wds_entity_manager {

    public:
        u8 reserved_000[0x38];
    };

    ASSERT_SIZEOF(wds_entity_manager, 0x38);
} // treyarch
