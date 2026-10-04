#pragma once

#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the typed parameter list behind it stays native for now
    class param_block {

    public:
        void*                 params;
        u32                   reserved_004;
        engine_recursive_lock lock;
        u32                   unk_018;
        bool                  unk_01c;
        u8                    pad_01d[0x03];
    };

    ASSERT_SIZEOF  (param_block,          0x20);
    ASSERT_OFFSETOF(param_block, params,  0x00);
    ASSERT_OFFSETOF(param_block, lock,    0x08);
    ASSERT_OFFSETOF(param_block, unk_018, 0x18);
    ASSERT_OFFSETOF(param_block, unk_01c, 0x1C);
} // treyarch
