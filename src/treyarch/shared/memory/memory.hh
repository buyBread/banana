#pragma once

#include "util/types.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace memory {
    void  __cdecl report(const char* format, ...);
    void* __cdecl allocate(u32 size, u32 alignment, u32 flags);
    void  __cdecl free(void* allocation);
}} // treyarch::memory
