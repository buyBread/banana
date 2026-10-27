#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "treyarch/base/tl_system.hh"

using namespace treyarch;

// sub_9CC940
void treyarch::tl_fatal(const char* format, ...) {
    char message[512];

    va_list arguments;
    va_start(arguments, format);
    std::vsprintf(message, format, arguments);
    va_end(arguments);

    auto critical_error = references::tl_cur_system_callbacks.get().critical_error;

    if (critical_error)
        critical_error(message);
}

// sub_9CC980
void* treyarch::tl_mem_alloc(u32 size, u32 align, u32 flags) {
    u32 effective_align = align;

    if (!align && !(size & 0x0F))
        effective_align = 0;

    ++references::tl_mem_alloc_counter.get();

    auto mem_alloc = references::tl_cur_system_callbacks.get().mem_alloc;

    void* allocation = mem_alloc ?
        mem_alloc(size, effective_align, flags) : _aligned_malloc(size, effective_align);

    if (!(flags & tl_memalloc_canfail) && !allocation)
        tl_fatal("Memory allocation failed. %d bytes, %d align", size, effective_align);

    return allocation;
}

// sub_9CC9F0
void treyarch::tl_mem_free(void* ptr) {
    --references::tl_mem_alloc_counter.get();

    auto mem_free = references::tl_cur_system_callbacks.get().mem_free;

    if (mem_free)
        mem_free(ptr);
    else
        _aligned_free(ptr);
}

// sub_9CCA90
void treyarch::tl_stack_range_init() {
    references::tl_stack_begin.write(nullptr);
    references::tl_stack_end  .write(nullptr);
}
