#pragma once

#include <cstddef>
#include <xmmintrin.h>

#include "util/types.hh"

namespace treyarch {
    // SM3's tl_defs.h
    template <typename T>
    inline T tl_align(T p, size_t a) {
        return (T)(((size_t)p + (a - 1)) & ~(a - 1));
    }

    inline void tl_prefetch(void* addr, u32 offset) {
        _mm_prefetch((const char*)&addr + offset, _MM_HINT_T0);
    }
} // treyarch
