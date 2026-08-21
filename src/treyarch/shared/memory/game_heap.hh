#pragma once

#include <windows.h>

#include "util/types.hh"

namespace treyarch { namespace memory { namespace game_heap {
    void* allocate_small_block(u32 size);
    void  free_small_block(void* allocation);
}}} // treyarch::memory::game_heap
