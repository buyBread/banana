#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;
    struct summon;

    struct summon_state {
        u8      reserved_000[0x14];
        summon* active_summon;
        u8      reserved_018[0x28];
        bool    end_requested;
        u8      reserved_041[0x07];
        u8      unk_048; // can_summon is false while it is set
        bool    summon_ability_script_toggled;
        u8      reserved_04a[0x4A];
        entity* current_stored_summon_actor;

        void deduct_meter_for_special_attack();
    };

    namespace references {
        // created through sub_951A20 by region_spawn_manager's constructor (sub_951A90)
        inline util::memory_reference<summon_state*> summon_state { 0x010FA238 };

        inline util::memory_reference<u8> unk_00be7455 { 0x00BE7455 };
    } // references

    ASSERT_OFFSETOF(summon_state, active_summon,                 0x14);
    ASSERT_OFFSETOF(summon_state, end_requested,                 0x40);
    ASSERT_OFFSETOF(summon_state, unk_048,                       0x48);
    ASSERT_OFFSETOF(summon_state, summon_ability_script_toggled, 0x49);
    ASSERT_OFFSETOF(summon_state, current_stored_summon_actor,   0x94);
} // treyarch
