#pragma once

#include <cstddef>

namespace treyarch {
    // SM3's tl_defs.h
    template <typename T>
    inline T tl_align(T p, size_t a) {
        return (T)(((size_t)p + (a - 1)) & ~(a - 1));
    }
} // treyarch
