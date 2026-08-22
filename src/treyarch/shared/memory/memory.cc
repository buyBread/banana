#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

using memory_error_callback      = void (__cdecl*)(const char* message);
using memory_allocation_callback = void*(__cdecl*)(u32 size, u32 alignment, u32 flags);
using memory_free_callback       = void (__cdecl*)(void* allocation);

static util::memory_reference<memory_error_callback>      memory_error_handler      { 0x01115A28 };
static util::memory_reference<memory_allocation_callback> memory_allocation_handler { 0x01115A34 };
static util::memory_reference<memory_free_callback>       memory_free_handler       { 0x01115A3C };
static util::memory_reference<u32>                        memory_allocation_count   { 0x01115A40 };

void __cdecl treyarch::memory::report(const char* format, ...) {
    char message[512];

    va_list arguments;
    va_start(arguments, format);
    std::vsprintf(message, format, arguments);
    va_end(arguments);

    memory_error_callback handler = memory_error_handler.read();

    if (handler)
        handler(message);
}

void* __cdecl treyarch::memory::allocate(u32 size, u32 alignment, u32 flags) {
    u32 effective_alignment = alignment;

    if (!alignment && !(size & 0x0F))
        effective_alignment = 0;

    ++memory_allocation_count.get();

    memory_allocation_callback handler = memory_allocation_handler.read();

    void* allocation = handler ?
        handler(size, effective_alignment, flags) : _aligned_malloc(size, effective_alignment);

    if (!(flags & 2) && !allocation)
        report("Memory allocation failed. %d bytes, %d align", size, effective_alignment);

    return allocation;
}

void __cdecl treyarch::memory::free(void* allocation) {
    --memory_allocation_count.get();

    memory_free_callback handler = memory_free_handler.read();

    if (handler)
        handler(allocation);
    else
        _aligned_free(allocation);
}
