#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct fixed_so_data_block_base;

    enum e_so_data_block_flags : u32 {
        so_data_block_flag_from_mash        = 0x01,
        so_data_block_flag_buffer_allocated = 0x02
    };

    // instance member data and the master script-variable storage both live in one of these
    struct so_data_block {
        u8*                       buffer;
        i32                       blocksize;
        e_so_data_block_flags     flags;
        fixed_so_data_block_base* fixed_block; // 32/128/512/1400/3000-byte pools; null for heap fallback
    };

    ASSERT_SIZEOF  (so_data_block,              0x10);
    ASSERT_OFFSETOF(so_data_block, buffer,      0x00);
    ASSERT_OFFSETOF(so_data_block, blocksize,   0x04);
    ASSERT_OFFSETOF(so_data_block, flags,       0x08);
    ASSERT_OFFSETOF(so_data_block, fixed_block, 0x0C);
}}} // treyarch::chuck::vm
