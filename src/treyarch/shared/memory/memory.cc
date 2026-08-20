#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

namespace treyarch { namespace memory {
    using error_callback      = void  (__cdecl*)(const char* message);
    using allocation_callback = void* (__cdecl*)(u32 size, u32 alignment, u32 flags);
    using free_callback       = void  (__cdecl*)(void* allocation);
    
namespace references {
        util::memory_reference<error_callback>      error_handler      { 0x01115A28 };
        util::memory_reference<allocation_callback> allocation_handler { 0x01115A34 };
        util::memory_reference<free_callback>       free_handler       { 0x01115A3C };
        util::memory_reference<u32>                 allocation_count   { 0x01115A40 };
} // references
}} // treyarch::memory

void __cdecl treyarch::memory::report(const char* format, ...) {
    char message[512];

    va_list arguments;
    va_start(arguments, format);
    std::vsprintf(message, format, arguments);
    va_end(arguments);

    error_callback handler = references::error_handler.read();

    if (handler)
        handler(message);
}

void* __cdecl treyarch::memory::allocate(u32 size, u32 alignment, u32 flags) {
    u32 effective_alignment = alignment;

    if (!alignment && !(size & 0x0F))
        effective_alignment = 0;

    ++references::allocation_count.get();

    allocation_callback handler = references::allocation_handler.read();

    void* allocation = handler ?
        handler(size, effective_alignment, flags) : _aligned_malloc(size, effective_alignment);

    if (!(flags & 2) && !allocation)
        report("Memory allocation failed. %d bytes, %d align", size, effective_alignment);

    return allocation;
}

void __cdecl treyarch::memory::free(void* allocation) {
    --references::allocation_count.get();

    free_callback handler = references::free_handler.read();

    if (handler)
        handler(allocation);
    else
        _aligned_free(allocation);
}
