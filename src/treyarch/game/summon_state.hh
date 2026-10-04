#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct summon_state {
        u8    reserved_000[0x14];
        void* active_summon;   // is_summon_summoned compares its name
        u8    reserved_018[0x28];
        u8    end_requested;   // end_summon sets it through sub_90FC00
        u8    reserved_041[0x07];
        u8    unk_048;         // can_summon is false while it is set
    };

    namespace references {
        // created through sub_951A20 by region_spawn_manager's constructor (sub_951A90)
        inline util::memory_reference<summon_state*> summon_state { 0x010FA238 };
    } // references

    ASSERT_OFFSETOF(summon_state, active_summon, 0x14);
    ASSERT_OFFSETOF(summon_state, end_requested, 0x40);
    ASSERT_OFFSETOF(summon_state, unk_048,       0x48);
} // treyarch
