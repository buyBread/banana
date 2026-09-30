#pragma once

#include "util/types.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace memory {
    enum e_allocation_flags : u32 {
        allocation_physical      = 0x00010000,
        allocation_write_combine = 0x00020000
    };

    using error_callback      = void (__cdecl*)(const char* message);
    using allocation_callback = void*(__cdecl*)(u32 size, u32 alignment, u32 flags);
    using free_callback       = void (__cdecl*)(void* allocation);

    void  report(const char* format, ...);
    void* allocate(u32 size, u32 alignment, u32 flags);
    void  free(void* allocation);

    namespace references {
        inline util::memory_reference<error_callback>      error_handler      { 0x01115A28 };
        inline util::memory_reference<allocation_callback> allocation_handler { 0x01115A34 };
        inline util::memory_reference<free_callback>       free_handler       { 0x01115A3C };
        inline util::memory_reference<u32>                 allocation_count   { 0x01115A40 };
    } // references
}} // treyarch::memory
