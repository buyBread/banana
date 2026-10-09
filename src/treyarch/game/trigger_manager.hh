#pragma once

#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class trigger_manager : public singleton_instance<trigger_manager, 0x0102FE48> {

    public:
        u32 unk_0000[2];        // the 8-byte type sub_95C220 zeroes
        u8  unk_0008;
        u8  unk_0009;
        u8  pad_000a[2];
        u32 unk_000c[2];        // same type
        u32 unk_0014[11];
        u32 unk_0040[1024][2];
        u32 unk_2040;

        trigger_manager();

        static void create_inst();
    };

    ASSERT_SIZEOF  (trigger_manager,           0x2044);
    ASSERT_OFFSETOF(trigger_manager, unk_000c, 0x000C);
    ASSERT_OFFSETOF(trigger_manager, unk_0040, 0x0040);
    ASSERT_OFFSETOF(trigger_manager, unk_2040, 0x2040);
} // treyarch
