#include <intrin.h>

#include "treyarch/mm/superblock.hh"

using namespace treyarch;

// sub_5F83B0
void* mm::superblock_entry_t::allocate() {
    long count;

    for (;;) { /* atomically take one of the unused blocks;
                  if there are none, the superblock is full */
        count = unused_blocks;

        if (!count)
            return nullptr;

        if (_InterlockedCompareExchange(&unused_blocks, count - 1, count) == count)
            break;
    }

    u32 word_index = bitmap_cursor;

    for (;;) {
        u64 occupied = (u64)bitmap[word_index];

        // U64_MAX means all 64 bits are 1 (fully occupied), so scan for a word with space
        while (occupied == U64_MAX) {
            if (++word_index >= bitmap_word_count)
                word_index = 0; // wrap around

            occupied = (u64)bitmap[word_index];
        }

        u64   available = ~occupied; // flip bits, 1s are now free slots
        ulong bit_index;
        u32   slot_in_word;

        // use CPU intrinsic to find the first set bit (fast way to find a free slot)
        if ((u32)(available >> 32)) {
            _BitScanReverse(&bit_index, (u32)(available >> 32));

            slot_in_word = 31 - bit_index;
        } else {
            _BitScanReverse(&bit_index, (u32)available);

            slot_in_word = 63 - bit_index;
        }

        // calculate bitmask for the specific slot and try to mark it occupied (1)
        u64 mask     = 1ull << (63 - slot_in_word);
        u64 exchange = occupied | mask;

        if (compare_exchange(&bitmap[word_index], exchange, occupied)) {
            bitmap_cursor = word_index; // cache this index for next time

            // calculate actual memory address based on slot index
            return superblock + block_size * (slot_in_word + (word_index << 6));
        }
    }
}

// sub_5F8510
void mm::superblock_entry_t::deallocate(void* allocation) {
    // figure out which bit belongs to this pointer
    u32 slot       = ((u8*)allocation - superblock) / block_size;
    u32 word_index = slot >> 6;                    // divide by 64 to find which u64 word it's in
    u64 mask       = 1ull << (63 - (slot & 0x3F)); // get specific bit in that word

    for (;;) { // atomically clear the bit (mark as free)
        auto occupied = (u64)bitmap[word_index];

        if (!(occupied & mask))
            return; // already-free check

        u64 exchange = occupied & ~mask;

        if (compare_exchange(&bitmap[word_index], exchange, occupied))
            break;
    }

    for (;;) { // atomically give the block back
        long count = unused_blocks;

        if (_InterlockedCompareExchange(&unused_blocks, count + 1, count) == count)
            return;
    }
}
