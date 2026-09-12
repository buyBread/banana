#pragma once

#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace memory {
    class fixed_pool_allocator;

    struct fixed_pool {
        void*                 block_list;
        void*                 free_list;
        u32                   slot_size;
        u32                   alignment;
        u32                   slots_per_block;
        u32                   block_count;
        u32                   initial_block_count;
        u32                   allocation_count;
        u32                   growable;
        fixed_pool_allocator* allocator;
        const char*           name;
        fixed_pool*           thread_next;
        engine_recursive_lock lock;
        bool                  initialized;
        u8                    padding_41[3];

        void initialize(const char* name,
                        u32         slot_size,
                        u32         alignment,
                        u32         slots_per_block,
                        u32         initial_block_count);

        void* allocate();
        void  release(void* allocation);

    private:
        void add_blocks(u32 count);
    };

    ASSERT_SIZEOF  (fixed_pool,                   0x44);
    ASSERT_OFFSETOF(fixed_pool, block_list,       0x00);
    ASSERT_OFFSETOF(fixed_pool, free_list,        0x04);
    ASSERT_OFFSETOF(fixed_pool, slot_size,        0x08);
    ASSERT_OFFSETOF(fixed_pool, alignment,        0x0C);
    ASSERT_OFFSETOF(fixed_pool, slots_per_block,  0x10);
    ASSERT_OFFSETOF(fixed_pool, block_count,      0x14);
    ASSERT_OFFSETOF(fixed_pool, allocation_count, 0x1C);
    ASSERT_OFFSETOF(fixed_pool, allocator,        0x24);
    ASSERT_OFFSETOF(fixed_pool, lock,             0x30);
    ASSERT_OFFSETOF(fixed_pool, initialized,      0x40);
}} // treyarch::memory
