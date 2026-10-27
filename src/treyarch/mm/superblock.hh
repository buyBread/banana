#pragma once

#include <intrin.h>

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mm {
    struct superheap_t;

    enum e_superblock_state : long {
        superblock_full   = 1,
        superblock_active = 2, // the size class allocates from it
        superblock_queued = 3  // in the size class's queue of superblocks with free blocks
    };

    // one 0x1000-byte superblock of equal blocks, tracked by a bitmap (set bits are taken)
    struct superblock_entry_t {
                 u32          block_size;
                 u32          total_blocks;
                 u32          bitmap_word_count;
                 u8*          superblock;
                 superheap_t* heap;
                 u32          bitmap_cursor;
        volatile long         unused_blocks;
        volatile long         state;
        volatile i64          bitmap[1];

        void* allocate();
        void  deallocate(void* allocation);
    };

    // atomic CAS, true if it swapped
    inline bool compare_exchange(volatile i64* destination,
                                          u64  exchange,
                                          u64  comparison) {

        return (u64)_InterlockedCompareExchange64(destination, (i64)exchange, (i64)comparison) == comparison;
    }

    ASSERT_SIZEOF  (superblock_entry_t,                    0x28);
    ASSERT_OFFSETOF(superblock_entry_t, block_size,        0x00);
    ASSERT_OFFSETOF(superblock_entry_t, total_blocks,      0x04);
    ASSERT_OFFSETOF(superblock_entry_t, bitmap_word_count, 0x08);
    ASSERT_OFFSETOF(superblock_entry_t, superblock,        0x0C);
    ASSERT_OFFSETOF(superblock_entry_t, heap,              0x10);
    ASSERT_OFFSETOF(superblock_entry_t, bitmap_cursor,     0x14);
    ASSERT_OFFSETOF(superblock_entry_t, unused_blocks,     0x18);
    ASSERT_OFFSETOF(superblock_entry_t, state,             0x1C);
    ASSERT_OFFSETOF(superblock_entry_t, bitmap,            0x20);
}} // treyarch::mm
