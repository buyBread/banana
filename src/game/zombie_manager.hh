#pragma once

#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class zombie_manager : public singleton<zombie_manager, 0x0102FFF0> {

    public:
        u8 reserved_004[0x69];
        u8 suspended; // set by chuck zombie_suspend
    };

    ASSERT_OFFSETOF(zombie_manager, suspended, 0x6D);
} // treyarch
