#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    // indexes the platform name tables starting at 0x00E82104
    enum e_platform : u32 {
        platform_ps3,
        platform_pc,
        platform_xenon
    };

    namespace references {
        inline util::memory_reference<e_platform> platform { 0x0102FD64 };
    } // references
} // treyarch
