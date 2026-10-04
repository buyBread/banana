#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class zombie_manager {

    public:
        u8 reserved_000[0x6D];
        u8 suspended; // set by chuck zombie_suspend
    };

    namespace references {
        inline util::memory_reference<zombie_manager*> zombies { 0x0102FFF0 };
    } // references

    ASSERT_OFFSETOF(zombie_manager, suspended, 0x6D);
} // treyarch
