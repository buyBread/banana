#pragma once

#include "treyarch/game/input/input_device.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class input_mgr {

    public:
        u8            reserved_000[0x820];
        input_device* devices[max_devices];

        void poll_devices();
    };

    namespace references {
        inline util::memory_reference<input_mgr*> input_manager { 0x010FC63C };
    } // references

    ASSERT_SIZEOF  (input_mgr,          0x848);
    ASSERT_OFFSETOF(input_mgr, devices, 0x820);
} // treyarch
