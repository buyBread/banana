#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

// sub_9CC940
void treyarch::memory::report(const char* format, ...) {
    char message[512];

    va_list arguments;
    va_start(arguments, format);
    std::vsprintf(message, format, arguments);
    va_end(arguments);

    error_callback handler = references::error_handler.read();

    if (handler)
        handler(message);
}

// sub_9CC980
void* treyarch::memory::allocate(u32 size, u32 alignment, u32 flags) {
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

// sub_9CC9F0
void treyarch::memory::free(void* allocation) {
    --references::allocation_count.get();

    free_callback handler = references::free_handler.read();

    if (handler)
        handler(allocation);
    else
        _aligned_free(allocation);
}
