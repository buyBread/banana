#pragma once

#include <windows.h>

#include "util/memory_reference.hh"
#include "util/types.hh"

struct heap_state;

namespace treyarch { namespace memory { namespace heap {
    void* allocate(u32 size);
    void  free(void* allocation);

    void* allocate_small_block(u32 size);
    void  free_small_block(void* allocation);

    namespace references {
        // created on first use by sub_5FB3D0
        inline util::memory_reference<heap_state*> heap_default { 0x00FFDA58 };
    } // references
}}} // treyarch::memory::heap
