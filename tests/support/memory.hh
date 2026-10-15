#pragma once

#include <cstddef>
#include <vector>

#include "util/types.hh"

namespace tests {
    // engine code has two sets of allocation functions; each allocation must be freed through the set that made it.
    enum e_allocator {
        heap_allocator,
        plain_allocator
    };

    // keep a record after the memory is freed; released_bytes saves its final contents for assertions made afterward.
    struct allocation_t {
        void*           pointer;
        u32             size;
        u32             alignment;
        u32             flags;
        e_allocator     allocator;
        bool            released;
        std::vector<u8> released_bytes;
    };

    struct free_call_t {
        void*       pointer;
        e_allocator allocator;
    };

    // a fixture supplies the setup and cleanup shared by a group of tests; each test gets a fresh instance.
    // this one records calls to the engine's allocation functions, so tests can check sizes and ownership.
    // it also frees leftover memory after the assertions; that cleanup does not count as an engine free call.
    class c_memory_fixture {
        c_memory_fixture* m_previous;

    public:
        std::vector<allocation_t> allocations;
        std::vector<free_call_t>  frees;
        
        u32 invalid_frees = 0;

        c_memory_fixture();
       ~c_memory_fixture();

        void* allocate(e_allocator allocator, u32 size, u32 alignment = 0, u32 flags = 0);
        void  free(e_allocator allocator, void* pointer);

        const allocation_t* find(const void* pointer) const;
        bool contains(const void* pointer) const;

        u32 allocation_count(e_allocator allocator) const;
        u32 free_count(e_allocator allocator) const;
        u32 live_count(e_allocator allocator) const;
    };

    extern c_memory_fixture* current_memory_fixture;
}
