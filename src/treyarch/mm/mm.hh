#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mm {
    struct memory_space;
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
        hint_largepages   = 0x08,
        hint_superheap    = 0x10
    };

    // what dlmalloc maps and unmaps its segments through
    struct memory_callbacks {
        memory_space* user_data;
        i32 (__cdecl* map)(u32 size, void* user_data);
        i32 (__cdecl* unmap)(void* address, u32 size, void* user_data);
    };

    // a heap: a dlmalloc space, with a superheap for small blocks when the hints ask for one
    struct memory_space {
                 char          name[32];
                 e_space_type  type;
                 u32           hint_flags;
                 u32           debug_flags;
        volatile long          spin_lock;
                 u32           reserved_030;
                 u32           size;
                 u32           used;
                 u32           reported_size;
                 u32           reported_used;
                 u32           report_threshold;
                 void*         space;
                 u32           initial_size;
                 u32           max_size;
                 superheap_t*  superheap;
                 memory_space* list_next;

        memory_space(const char* space_name, e_space_type space_type, u32 hints, u32 capacity, void* base);
        ~memory_space();

        void* allocate(u32 bytes, u32 alignment);
        void* reallocate(void* allocation, u32 bytes, u32 alignment);
        void  deallocate(void* allocation);
        u32   allocation_size(void* allocation);

        void lock();
        void unlock();

        static i32 map_memory(u32 bytes, void* user_data);
        static i32 unmap_memory(void* address, u32 bytes, void* user_data);

        static void allocation_failure(void* failed_space);
        static void report_usage(bool force);
    };

    void          init();
    memory_space* get_default_space(); // creates it on first use

    /*
        mm replaces the global operator new and operator delete: every new and delete in the game,
        arrays and the VC8 containers' std::allocator included, allocates from the default space.

        source can't replace its own operators without moving all of its allocations onto the
        game heap as well, so these two stand in for them wherever retail calls new or delete
    */
    void* allocate(u32 size);
    void  free(void* allocation);

    // the library callbacks' entries: tl_mem_alloc and tl_mem_realloc lead here
    void* allocate(u32 size, u32 alignment);
    void* reallocate(void* allocation, u32 size, u32 alignment);

    // skips the superheap
    void* allocate_general(u32 size, u32 alignment);

    void get_virtual_memory_usage(u32* total, u32* used);
    void load_module_information();

    namespace references {
        // created on first use by sub_5FB3D0
        inline util::memory_reference<memory_space*> default_space { 0x00FFDA58 };
        inline util::memory_reference<memory_space*> list_head     { 0x00FFDA5C };
    } // references

    ASSERT_OFFSETOF(memory_space, type,             0x20);
    ASSERT_OFFSETOF(memory_space, hint_flags,       0x24);
    ASSERT_OFFSETOF(memory_space, debug_flags,      0x28);
    ASSERT_OFFSETOF(memory_space, spin_lock,        0x2C);
    ASSERT_OFFSETOF(memory_space, size,             0x34);
    ASSERT_OFFSETOF(memory_space, used,             0x38);
    ASSERT_OFFSETOF(memory_space, reported_size,    0x3C);
    ASSERT_OFFSETOF(memory_space, reported_used,    0x40);
    ASSERT_OFFSETOF(memory_space, report_threshold, 0x44);
    ASSERT_OFFSETOF(memory_space, space,            0x48);
    ASSERT_OFFSETOF(memory_space, initial_size,     0x4C);
    ASSERT_OFFSETOF(memory_space, max_size,         0x50);
    ASSERT_OFFSETOF(memory_space, superheap,        0x54);
    ASSERT_OFFSETOF(memory_space, list_next,        0x58);

    ASSERT_SIZEOF  (memory_callbacks,            0x0C);
    ASSERT_OFFSETOF(memory_callbacks, map,       0x04);
    ASSERT_OFFSETOF(memory_callbacks, unmap,     0x08);
}} // treyarch::mm
