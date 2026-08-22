#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace list {
    struct arena_state {
        u8* base;
        u32 capacity;
        u8* cursor;
    };

    void* allocate(u32 size, u32 alignment);
    void  rewind();

    namespace references {
        inline util::memory_reference<arena_state> arena { 0x01115C64 };
    } // references

    ASSERT_SIZEOF  (arena_state,           0x0C);
    ASSERT_OFFSETOF(arena_state, base,     0x00);
    ASSERT_OFFSETOF(arena_state, capacity, 0x04);
    ASSERT_OFFSETOF(arena_state, cursor,   0x08);
}}} // treyarch::ngl::list
