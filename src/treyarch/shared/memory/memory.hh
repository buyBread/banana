#pragma once

#include "util/types.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace memory {
    enum e_allocation_flags : u32 {
        allocation_physical      = 0x00010000,
        allocation_write_combine = 0x00020000
    };

    void  report(const char* format, ...);
    void* allocate(u32 size, u32 alignment, u32 flags);
    void  free(void* allocation);
}} // treyarch::memory
