#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mm {
    struct superheap_t;

    enum e_space_type : u32 {
        type_physical,
        type_virtual,
        type_vram,
        type_fixed
    };

    enum e_hint_flag : u32 {
        hint_none         = 0x00,
        hint_top          = 0x01,
        hint_nocache      = 0x02,
        hint_writecombine = 0x04,
        hint_largepages   = 0x08
    };

    // a heap: a dlmalloc space, with a superheap for small blocks when the hints ask for one
    struct memory_space {
        char          name[32];
        e_space_type  type;
        u32           hint_flags;
        u8            reserved_028[0x10];
        u32           super_used;
        u8            reserved_03c[0x0C];
        void*         space;
        u32           initial_size;
        u32           max_size;
        superheap_t*  superheap;
        memory_space* list_next;
    };

    /*
        mm replaces the global operator new and operator delete: every new and delete in the game,
        arrays and the VC8 containers' std::allocator included, allocates from the default space.

        source can't replace its own operators without moving all of its allocations onto the
        game heap as well, so these two stand in for them wherever retail calls new or delete
    */
    void* allocate(u32 size);
    void  free(void* allocation);

    namespace references {
        // created on first use by sub_5FB3D0
        inline util::memory_reference<memory_space*> default_space { 0x00FFDA58 };
    } // references

    ASSERT_OFFSETOF(memory_space, type,         0x20);
    ASSERT_OFFSETOF(memory_space, hint_flags,   0x24);
    ASSERT_OFFSETOF(memory_space, super_used,   0x38);
    ASSERT_OFFSETOF(memory_space, space,        0x48);
    ASSERT_OFFSETOF(memory_space, initial_size, 0x4C);
    ASSERT_OFFSETOF(memory_space, max_size,     0x50);
    ASSERT_OFFSETOF(memory_space, superheap,    0x54);
    ASSERT_OFFSETOF(memory_space, list_next,    0x58);
}} // treyarch::mm
