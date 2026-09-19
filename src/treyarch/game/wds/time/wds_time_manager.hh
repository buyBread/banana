#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class wds_time_manager {

    public:
        u8  reserved_000[0x1C];
        u32 frame_sequence;
        u8  reserved_020[0x1C];
    };

    ASSERT_SIZEOF  (wds_time_manager,                 0x3C);
    ASSERT_OFFSETOF(wds_time_manager, frame_sequence, 0x1C);
} // treyarch
