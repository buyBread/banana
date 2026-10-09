#pragma once

#include "treyarch/game/input/input_device.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class input_mgr : public singleton<input_mgr, 0x010FC63C> {

    public:
        void*         unk_004;
        u8            reserved_008[0x804];
        u8            unk_80c; // cleared by game::unload_current_level
        u8            reserved_80d[0x13];
        input_device* devices[max_devices];

        void poll_devices();
    };

    ASSERT_SIZEOF  (input_mgr,          0x848);
    ASSERT_OFFSETOF(input_mgr, unk_004, 0x004);
    ASSERT_OFFSETOF(input_mgr, unk_80c, 0x80C);
    ASSERT_OFFSETOF(input_mgr, devices, 0x820);
} // treyarch
