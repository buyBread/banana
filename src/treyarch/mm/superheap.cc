#include <windows.h>
#include <intrin.h>
#include <cstring>

#include "retail.hh"
#include "treyarch/mm/superheap.hh"
#include "banana/util/memory_reference.hh"

/*
    https://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf
    https://en.wikipedia.org/wiki/Treiber_stack
    https://en.wikipedia.org/wiki/ABA_problem
*/

using namespace treyarch;

namespace treyarch { namespace mm { namespace references {
    util::memory_reference<superblock_entry_t**> superblock_table { 0x00FFDA70 };
    util::memory_reference<u8*>                  free_superblocks { 0x00FFDE74 };

    util::memory_reference<volatile long> superblock_lock { 0x00FFDE70 };
    util::memory_reference<volatile i64>  queue_node_pool { 0x00FFE1D8 };
}}} // treyarch::mm::references

/*
    ABA problem prevention;
    lock-free structures fail if a pointer is freed and reallocated between a thread's read and CAS (compare & swap);
    packing a 32-bit pointer and a 32-bit sequence counter to a 64-bit integer guarantees uniqueness
*/

static u64 make_tagged_pointer(const void* pointer, u32 tag) {
    return ((u64)tag << 32) | (u32)pointer;
}

static void* tagged_pointer(u64 value) {
    return (void*)(u32)value; // bottom 32 bits
}

static u32 tagged_sequence(u64 value) {
    return (u32)(value >> 32); // top 32 bits
}

/*
    spinlock for superblock creation and destruction
*/

static void acquire_superblock_lock() {
    volatile long &lock = mm::references::superblock_lock.get();

    while (_InterlockedCompareExchange(&lock, 1, 0) != 0)
        Sleep(0); // could be `std::this_thread::yield()` probably?
}

static void release_superblock_lock() {
    volatile long &lock = mm::references::superblock_lock.get();

    // reset lock to 0
    while (_InterlockedCompareExchange(&lock, 0, 1) != 1) {}
}

/*
    treiber stack
*/

// sub_5FB7D0
static mm::superblock_queue_node* acquire_queue_node() {
    volatile i64 &pool = mm::references::queue_node_pool.get();

    for (;;) {
        auto  state = (u64)pool;
        auto* node  = (mm::superblock_queue_node*)tagged_pointer(state);

        if (!node) {
            // slice a block from the default space into 256 linked queue nodes
            auto* nodes = (mm::superblock_queue_node*)retail::sub_5FAF00((i32)mm::get_default_space(), 0x1000, 0);

            for (u32 index = 0; index < 0xFF; ++index)
                nodes[index].next = (u32)&nodes[index + 1];

            nodes[0xFF].next = (u32)tagged_pointer(state);

            // atomic push onto the stack
            u64 next = make_tagged_pointer(nodes, tagged_sequence(state) + 1);

            if (!mm::compare_exchange(&pool, next, state)) // someone else beat us, discard our batch
                retail::sub_5FAFB0((volatile i32*)mm::get_default_space(), (i32)nodes);

            continue;
        }

        u64 next = make_tagged_pointer((void*)(u32)node->next, tagged_sequence(state) + 1);

        // lock-free pop
        if (mm::compare_exchange(&pool, next, state))
            return node;
    }
}

// sub_5FAE10
static void release_queue_node(mm::superblock_queue_node* node) {
    volatile i64 &pool = mm::references::queue_node_pool.get();

    for (;;) {
        auto state = (u64)pool;
        node->next = (u32)tagged_pointer(state);
        auto  next = make_tagged_pointer(node, tagged_sequence(state) + 1);

        // lock-free push
        if (mm::compare_exchange(&pool, next, state))
            return;
    }
}

// sub_5FAEB0
mm::superheap_t::superheap_t() {
    // each size class's queue starts with a dummy node
    for (size_class &block_class : size_classes) {
        block_class.available.head = 0;
        block_class.available.tail = 0;

        superblock_queue_node* node = acquire_queue_node();

        node->next = 0;

        block_class.available.tail = (u32)node;
        block_class.available.head = (u32)node;
        block_class.block_size     = 0;
        block_class.active         = nullptr;
        block_class.unused_entries = nullptr;
    }
}

/*
    Michael-Scott lock-free queue
*/

// sub_5FB0E0
bool mm::superblock_queue::dequeue(superblock_entry_t** output) {
    for (;;) {
        auto  head_state = (u64)head;
        auto  tail_state = (u64)tail;
        auto* head_node  = (superblock_queue_node*)tagged_pointer(head_state);
        auto  next       = (u64)head_node->next;

        if (head_state != (u64)head)
            continue; // ensure head's the same

        // queue empty or tail is lagging behind
        if (tagged_pointer(head_state) == tagged_pointer(tail_state)) {
            if (!tagged_pointer(next))
                return false; // queue empty

            /*
                tail is lagging;
                help advance it before trying to dequeue again
            */
            compare_exchange(&tail,
                             make_tagged_pointer(tagged_pointer(next),
                                                 tagged_sequence(tail_state) + 1),
                             tail_state);
            continue;
        }

        auto* next_node = (superblock_queue_node*)tagged_pointer(next);

        *output = next_node->value;

        // try to move the head to the next node
        if (compare_exchange(&head,
                             make_tagged_pointer(next_node,
                                                 tagged_sequence(head_state) + 1),
                             head_state)) {

            release_queue_node(head_node); // recycle old dummy head node

            return true;
        }
    }
}

// sub_5FB940
void mm::superblock_queue::enqueue(superblock_entry_t* value) {
    auto* node = acquire_queue_node();

    node->value = value;
    node->next  = 0;

    for (;;) {
        auto  tail_state = (u64)tail;
        auto* tail_node  = (superblock_queue_node*)tagged_pointer(tail_state);
        auto  next       = (u64)tail_node->next;

        if (tail_state != (u64)tail)
            continue;

        /*
            tail is lagging;
            help advance it before trying to enqueue again
        */
        if (tagged_pointer(next)) {
            compare_exchange(&tail,
                             make_tagged_pointer(tagged_pointer(next),
                                                 tagged_sequence(tail_state) + 1),
                             tail_state);
            continue;
        }

        // try to link our new node at the end of the list
        if (compare_exchange(&tail_node->next,
                             make_tagged_pointer(node,
                                                 tagged_sequence(next) + 1),
                             next)) {

            /*
                success, now point the tail to our new node;
                another thread will help us do it later if we fail...
            */
            compare_exchange(&tail,
                             make_tagged_pointer(node,
                                                 tagged_sequence(tail_state) + 1),
                             tail_state);
            return;
        }
    }
}

// sub_5FAB80
u32 mm::superheap_t::get_block_size(u32 size, u32 alignment) { // 0 when no size class serves the request
    if (size > 0x100)
        return 0;

    size_class* block_class = get_size_class(size);

    if (alignment && block_class->block_size % alignment)
        return 0;

    return block_class->block_size;
}

/*
    retrieves the size class for a specific allocation size;
    sizes 1-4 bytes -> index 0; sizes 5-8 bytes -> index 1
*/
// inlined @ sub_5FAB80
// inlined @ sub_5FB4F0
// inlined @ sub_5FBB00
// inlined @ sub_5FBC20
// inlined @ sub_5F8650
mm::size_class* mm::superheap_t::get_size_class(u32 block_size) {
    u32 clamped_size = block_size < 4 ? 4 : block_size;
    u32 index        = ((clamped_size + 3) >> 2) - 1;

    return &size_classes[index];
}

/*
    Radix Tree;
    maps arbitrary raw memory pointers back to their owning `superblock_entry_t`,
    required so `deallocate()` knows where metadata is without storing headers in front of allocations
    ...which would waste cache space
*/

// sub_5FB470
void mm::superheap_t::register_superblock(superblock_entry_t* entry) {
    auto                   address = (u32)entry->superblock;
    superblock_entry_t***  table   = &references::superblock_table.get();
    superblock_entry_t**  &level   = table[address >> 24];

    if (!level) {
        level = (superblock_entry_t**)retail::sub_5FAF00((i32)get_default_space(), 0x4000, 0);

        std::memset(level, 0, 0x4000);
    }

    // middle 12 bits ( 0x1000 aligned superblock addresses )
    level[(address >> 12) & 0x0FFF] = entry;
}

// inlined @ sub_5F8650
void mm::superheap_t::unregister_superblock(superblock_entry_t* entry) {
    auto                  address = (u32)entry->superblock;
    superblock_entry_t*** table   = &references::superblock_table.get();

    table[address >> 24][(address >> 12) & 0x0FFF] = nullptr;
}

// inlined @ sub_5FBD70
// inlined @ sub_5FBC20
mm::superblock_entry_t* mm::find_superblock_entry(void* allocation) {
    // traverse the radix tree to find the metadata for this pointer
    auto                  address = (u32)allocation;
    superblock_entry_t*** table   = &references::superblock_table.get();
    superblock_entry_t**  level   = table[address >> 24];

    if (!level)
        return nullptr;

    return level[(address >> 12) & 0x0FFF];
}

// sub_5FB4F0
mm::superblock_entry_t* mm::superheap_t::create_superblock_entry(u32 block_size, u8* superblock) {
    size_class* block_class = get_size_class(block_size);

    u32 total_blocks           = 0x1000 / block_size;
    u32 full_bitmap_word_count = total_blocks >> 6;
    u32 partial_bitmap_bits    = total_blocks & 0x3F;
    u32 bitmap_word_count      = full_bitmap_word_count + (partial_bitmap_bits ? 1 : 0);

    // recycled entries come first
    superblock_entry_t* entry = block_class->unused_entries;

    if (entry)
        block_class->unused_entries = (superblock_entry_t*)entry->block_size;
    else {
        // otherwise carve one from the entry slab
        u32 entry_size   = 0x28 + bitmap_word_count * sizeof(i64);
        u32 aligned_size = (entry_size + 7) & ~7u;

        superblock_entry_slab* slab = entry_allocator;

        if (!slab || slab->used + aligned_size >= 0x4000) {
            slab = (superblock_entry_slab*)retail::sub_5FAF00((i32)get_default_space(), sizeof(superblock_entry_slab), 0);

            if (slab) {
                slab->previous = entry_allocator;
                slab->used     = (u32)slab & 7 ? 8 - ((u32)slab & 7) : 0;
            }

            entry_allocator = slab;
        }

        entry = (superblock_entry_t*)(slab->storage + slab->used);

        slab->used += aligned_size;
    }

    entry->block_size        = block_size;
    entry->total_blocks      = total_blocks;
    entry->bitmap_word_count = full_bitmap_word_count;
    entry->superblock        = superblock;
    entry->heap              = this;
    entry->bitmap_cursor     = 0;
    entry->unused_blocks     = total_blocks;
    entry->state             = superblock_active;

    std::memset((void*)entry->bitmap, 0, full_bitmap_word_count * sizeof(i64));

    // if the 0x1000 space doesn't perfectly divide by the block size, mark the trailing "garbage" bits as permanently occupied so we don't allocate them
    if (partial_bitmap_bits) {
        entry->bitmap[full_bitmap_word_count] = (1ull << (64 - partial_bitmap_bits)) - 1;

        ++entry->bitmap_word_count;
    }

    return entry;
}

// sub_5F8600
void mm::superheap_t::initialize(memory_space* owner) {
    space = owner;

    references::superblock_lock.get() = 0;
    references::free_superblocks.write(nullptr);

    entry_allocator = nullptr;

    std::memset(&references::superblock_table.get(), 0, 0x100 * sizeof(superblock_entry_t**));

    // four-byte size classes, 4 through 0x100 bytes
    for (u32 index = 0; index < 64; ++index)
        size_classes[index].block_size = 4 * (index + 1);
}

// inlined @ sub_5F8650
void mm::superheap_t::destroy_superblock_entry(superblock_entry_t* entry) {
    // back to the size class's recycled entries, linked through block_size
    size_class* block_class = get_size_class(entry->block_size);

    entry->block_size           = (u32)block_class->unused_entries;
    block_class->unused_entries = entry;
}

// sub_5F8700
void mm::superheap_t::get_usage(u32* used, u32* size) {
    acquire_superblock_lock();

    *used = 0;
    *size = 0;

    superblock_entry_t*** table = &references::superblock_table.get();

    // every registered superblock counts its size and the blocks in use
    for (u32 high = 0; high < 0x100; ++high) {
        if (!table[high])
            continue;

        for (u32 middle = 0; middle < 0x1000; ++middle) {
            superblock_entry_t* entry = table[high][middle];

            if (!entry)
                continue;

            *size += 0x1000;
            *used += entry->block_size * (entry->total_blocks - entry->unused_blocks);
        }
    }

    // and the free superblocks their size alone
    for (u8* superblock = references::free_superblocks.read(); superblock; superblock = *(u8**)superblock)
        *size += 0x1000;

    release_superblock_lock();
}

// sub_5F8820
void mm::superheap_t::dump() {
    acquire_superblock_lock();
    release_superblock_lock();
}

// sub_5FB690
mm::superblock_entry_t* mm::superheap_t::create_superblock(u32 block_size) {
    acquire_superblock_lock();

    /*
        game maps 0x40000 chunks from the OS;
        then manually partitions them into 0x1000 superblocks to feed this system...
    */
    u8* superblock = references::free_superblocks.read();

    if (superblock)
        references::free_superblocks.write(*(u8**)superblock);
    else {
        superblock = (u8*)memory_space::map_memory(0x40000, space);

        // the space reports the failure with the lock released
        if (superblock == (u8*)-1) {
            release_superblock_lock();
            memory_space::allocation_failure(space->space);
            acquire_superblock_lock();
        }

        u8* free_superblock = references::free_superblocks.read();

        // slice a 0x40000 chunk into 0x1000 superblocks and link them via their first word
        for (u32 offset = 0x1000; offset < 0x40000; offset += 0x1000) {
            u8* next_superblock = superblock + offset;

            *(u8**)next_superblock = free_superblock;

            free_superblock = next_superblock;
        }

        references::free_superblocks.write(free_superblock);
    }

    superblock_entry_t* entry = create_superblock_entry(block_size, superblock);

    register_superblock(entry);
    release_superblock_lock();

    return entry;
}

// sub_5F8650
void mm::superheap_t::destroy_superblock(superblock_entry_t* entry) {
    acquire_superblock_lock();

    unregister_superblock(entry);

    // return the 0x1000 superblock to the free list
    *(u8**)entry->superblock = references::free_superblocks.read();

    references::free_superblocks.write(entry->superblock);

    destroy_superblock_entry(entry);
    release_superblock_lock();
}

// sub_5FBB00
void* mm::superheap_t::allocate(u32 size) {
    size_class* block_class = get_size_class(size);

    for (;;) {
        // fast path: we have a designated active superblock for this size class
        for (superblock_entry_t* entry = block_class->active; entry; entry = block_class->active) {
            void* allocation = entry->allocate();

            if (allocation)
                return allocation;

            /*
                superblock is full...
                transition it from active to full, then remove it as the active one so we fetch a new one
            */
            if (_InterlockedCompareExchange(&entry->state, superblock_full, superblock_active) == superblock_active)
                _InterlockedCompareExchange((volatile long*)&block_class->active,
                                            0,
                                            (long)entry);
        }

        // slooow path: no active superblock, pull partially full ones from the queue
        superblock_entry_t* entry;

        while (block_class->available.dequeue(&entry)) {
            void* allocation = entry->allocate();

            if (!allocation) {
                entry->state = superblock_full; // it was in the queue, but is actually full

                continue;
            }

            entry->state = superblock_active; // we succesfully pulled from it

            // make it the active superblock for this size class
            if (_InterlockedCompareExchange((volatile long*)&block_class->active, (long)entry, 0)) {
                // another thread set an active superblock first, queue it again
                entry->state = superblock_queued;

                block_class->available.enqueue(entry);
            }

            return allocation;
        }

        // even sloooooower path: queue is empty, create a brand new superblock
        entry = create_superblock(block_class->block_size);

        // attempt to install as active superblock; if another thread beat us, release and try again
        if (_InterlockedCompareExchange((volatile long*)&block_class->active, (long)entry, 0))
            destroy_superblock(entry);
    }
}

// sub_5FBC20
void mm::superheap_t::deallocate(void* allocation, superblock_entry_t* entry) {
    if (!entry)
        entry = find_superblock_entry(allocation);

    entry->deallocate(allocation);

    size_class* block_class = get_size_class(entry->block_size);

    // if the superblock was full, it now has space; queue it up
    if (_InterlockedCompareExchange(&entry->state, superblock_queued, superblock_full) == superblock_full)
        block_class->available.enqueue(entry);

    // if the superblock is not in the queue or still has blocks in use, we're done
    if (entry->state != superblock_queued || entry->unused_blocks != (long)entry->total_blocks)
        return;

    /*
        we should give this 4KiB (0x1000) superblock back to prevent memory hoarding;
        because it's in a queue, we have to cycle the queue until we find it...
    */
    for (u32 scan_count = 0; scan_count < 0x100; ++scan_count) {
        superblock_entry_t* queued;

        if (!block_class->available.dequeue(&queued))
            return; // queue is empty, abort

        if (queued != entry) {
            // this isn't our superblock, put it back
            block_class->available.enqueue(queued);

            continue;
        }

        // hello there, let's check if it's empty
        if (queued->state == superblock_queued && queued->unused_blocks == (long)queued->total_blocks)
            // it is, bye bye
            destroy_superblock(queued);
        else
            // another thread allocated from it while we were looking for it, put it back
            block_class->available.enqueue(queued);

        return;
    }
}
