#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class glass_house_manager {

    public:
        u8  reserved_000[0x10];
        i32 glass_house_level;
        u8  reserved_014[0x04];
    };

    namespace references {
        inline util::memory_reference<glass_house_manager*> glass_house_manager { 0x0102FEC4 };
    } // references

    ASSERT_SIZEOF  (glass_house_manager,                    0x18);
    ASSERT_OFFSETOF(glass_house_manager, glass_house_level, 0x10);
} // treyarch
