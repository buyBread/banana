#include <intrin.h> // you already know we're getting evil in here
#include <cstdlib>
#include <cstring>

#include "treyarch/shared/memory/game_heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

struct game_small_block_heap; // fwd

struct game_heap_page {
             u32                    block_size;
             u32                    block_count;
             u32                    bitmap_word_count;
             u8*                    blocks;
             game_small_block_heap* owner;
             u32                    bitmap_cursor;
    volatile LONG                   free_count;
    volatile LONG                   state;
    volatile i64                    bitmap[1];
};

struct game_heap_page_queue_node {
    volatile i64             next;
             game_heap_page* value;
             u32             reserved_00c;
};

struct game_heap_page_queue {
    volatile i64 head;
    volatile i64 tail;
};

struct game_heap_size_class {
    u32                  block_size;
    game_heap_page*      active_page;
    game_heap_page_queue available_pages;
    game_heap_page*      unused_pages;
    u32                  reserved_01c;
};

struct game_heap_metadata_slab {
    game_heap_metadata_slab* previous;
    u32                      used;
    u8                       storage[0x4000];
};

struct game_small_block_heap {
    void*                    owner;
    u32                      reserved_004;
    game_heap_size_class     size_classes[64];
    game_heap_metadata_slab* metadata_slabs;
};

struct game_heap_state {
    u8                     reserved_000[0x38];
    u32                    small_block_bytes;
    u8                     reserved_03c[0x18];
    game_small_block_heap* small_block_heap;
};

ASSERT_SIZEOF  (game_heap_page,                    0x28);
ASSERT_OFFSETOF(game_heap_page, block_size,        0x00);
ASSERT_OFFSETOF(game_heap_page, block_count,       0x04);
ASSERT_OFFSETOF(game_heap_page, bitmap_word_count, 0x08);
ASSERT_OFFSETOF(game_heap_page, blocks,            0x0C);
ASSERT_OFFSETOF(game_heap_page, owner,             0x10);
ASSERT_OFFSETOF(game_heap_page, bitmap_cursor,     0x14);
ASSERT_OFFSETOF(game_heap_page, free_count,        0x18);
ASSERT_OFFSETOF(game_heap_page, state,             0x1C);
ASSERT_OFFSETOF(game_heap_page, bitmap,            0x20);

ASSERT_SIZEOF  (game_heap_page_queue_node,        0x10);
ASSERT_OFFSETOF(game_heap_page_queue_node, next,  0x00);
ASSERT_OFFSETOF(game_heap_page_queue_node, value, 0x08);

ASSERT_SIZEOF  (game_heap_size_class,                  0x20);
ASSERT_OFFSETOF(game_heap_size_class, block_size,      0x00);
ASSERT_OFFSETOF(game_heap_size_class, active_page,     0x04);
ASSERT_OFFSETOF(game_heap_size_class, available_pages, 0x08);
ASSERT_OFFSETOF(game_heap_size_class, unused_pages,    0x18);

ASSERT_SIZEOF  (game_heap_metadata_slab,           0x4008);
ASSERT_OFFSETOF(game_heap_metadata_slab, previous, 0x0000);
ASSERT_OFFSETOF(game_heap_metadata_slab, used,     0x0004);
ASSERT_OFFSETOF(game_heap_metadata_slab, storage,  0x0008);

ASSERT_SIZEOF  (game_small_block_heap,                 0x810);
ASSERT_OFFSETOF(game_small_block_heap, size_classes,   0x008);
ASSERT_OFFSETOF(game_small_block_heap, metadata_slabs, 0x808);
ASSERT_OFFSETOF(game_heap_state, small_block_bytes, 0x38);
ASSERT_OFFSETOF(game_heap_state, small_block_heap,  0x54);

using game_allocation_callback = void*(__cdecl*)(u32 size, u32 alignment, u32 flags);
using game_free_callback       = void (__cdecl*)(void* allocation);

static util::memory_reference<game_heap_state*>         game_heap_default             { 0x00FFDA58 };
static util::memory_reference<u32>                      game_heap_page_directories    { 0x00FFDA70 };
static util::memory_reference<volatile i64>             game_heap_queue_node_pool     { 0x00FFE1D8 };
static util::memory_reference<volatile LONG>            game_heap_page_lock           { 0x00FFDE70 };
static util::memory_reference<u8*>                      game_heap_free_pages          { 0x00FFDE74 };
static util::memory_reference<game_allocation_callback> game_heap_allocation_callback { 0x01115A34 };
static util::memory_reference<game_free_callback>       game_heap_free_callback       { 0x01115A3C };

/*
    i'm severely out of my depth here and i don't ever want to touch this again.
    even after eventually getting it to work ((now)), everything here is (mostly) bogus code to me without any comments.
    probably because the process included ChatGPT re-explaining the same concepts six or seven times to me.

    https://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf
    https://en.wikipedia.org/wiki/Treiber_stack
    https://en.wikipedia.org/wiki/ABA_problem
*/

using namespace treyarch;
using namespace treyarch::memory;

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

static bool compare_exchange(volatile i64* destination,
                                      u64  exchange,
                                      u64  comparison) {

    // atomic CAS, true if success
    return (u64)_InterlockedCompareExchange64(destination, (i64)exchange, (i64)comparison) == comparison;
}

/*
    spinlocks for page creation
*/

static void acquire_page_lock() {
    volatile LONG &lock = game_heap_page_lock.get();

    while (_InterlockedCompareExchange(&lock, 1, 0) != 0)
        Sleep(0); // could be `std::this_thread::yield()` probably?
                  // Sleep(0) yields the thread's time slice for other processes
}

static void release_page_lock() {
    volatile LONG &lock = game_heap_page_lock.get();

    // reset lock to 0
    while (_InterlockedCompareExchange(&lock, 0, 1) != 1) {}
}

/*
    game's allocator callbacks
*/

static void* allocate_game_heap_storage(u32 size, u32 alignment) {
    auto callback = game_heap_allocation_callback.read();

    if (!callback)
        std::abort();

    void* allocation = callback(size, alignment, 0);

    if (!allocation)
        std::abort();

    return allocation;
}

static void free_game_heap_storage(void* allocation) {
    auto callback = game_heap_free_callback.read();

    if (!callback)
        std::abort();

    callback(allocation);
}

/*
    treiber stack
*/

static void refill_queue_node_pool() {
    volatile i64 &pool  = game_heap_queue_node_pool.get();
             u64  state = (u64)pool;

    if (tagged_pointer(state))
        return; // if another thread has refilled it, bail

    // allocate a page, slice it into linked 256 queue nodes
    auto* nodes = (game_heap_page_queue_node*)allocate_game_heap_storage(0x1000, 0);

    for (u32 index = 0; index < 0xFF; ++index)
        nodes[index].next = (u32)&nodes[index + 1];

    nodes[0xFF].next = (u32)tagged_pointer(state);

    // atomic push onto the stack
    u64 next = make_tagged_pointer(nodes, tagged_sequence(state) + 1);

    if (!compare_exchange(&pool, next, state))
        free_game_heap_storage(nodes); // someone else beat us, discard our batch
}

static game_heap_page_queue_node* acquire_queue_node() {
    volatile i64 &pool = game_heap_queue_node_pool.get();

    for (;;) {
        auto  state = (u64)pool;
        auto* node  = (game_heap_page_queue_node*)tagged_pointer(state);

        if (!node) {
            refill_queue_node_pool();
            
            continue;
        }

        u64 next = make_tagged_pointer((void*)(u32)node->next, tagged_sequence(state) + 1);

        // lock-free pop
        if (compare_exchange(&pool, next, state))
            return node;
    }
}

static void release_queue_node(game_heap_page_queue_node* node) {
    volatile i64 &pool = game_heap_queue_node_pool.get();

    for (;;) {
        auto state = (u64)pool;
        node->next = (u32)tagged_pointer(state);
        auto  next = make_tagged_pointer(node, tagged_sequence(state) + 1);

        // lock-free push
        if (compare_exchange(&pool, next, state))
            return;
    }
}

/*
    Michael-Scott lock-free queue
*/

static bool dequeue_page(game_heap_page_queue* queue,
                         game_heap_page**      output) {

    for (;;) {
        auto  head      = (u64)queue->head;
        auto  tail      = (u64)queue->tail;
        auto* head_node = (game_heap_page_queue_node*)tagged_pointer(head);
        auto  next      = (u64)head_node->next;

        if (head != (u64)queue->head)
            continue; // ensure head's the same

        // queue empty or tail is lagging behind
        if (tagged_pointer(head) == tagged_pointer(tail)) {
            if (!tagged_pointer(next))
                return false; // queue empty

            /*
                tail is lagging;
                help advance it before trying to dequeue again
            */
            compare_exchange(&queue->tail,
                             make_tagged_pointer(tagged_pointer(next),
                                                 tagged_sequence(tail) + 1),
                             tail);
            continue;
        }

        auto* next_node = (game_heap_page_queue_node*)tagged_pointer(next);
        
        *output = next_node->value;

        // try to move the head to the next node
        if (compare_exchange(&queue->head,
                             make_tagged_pointer(next_node,
                                                 tagged_sequence(head) + 1),
                             head)) {

            release_queue_node(head_node); // recycle old dummy head node
            
            return true;
        }
    }
}

static void enqueue_page(game_heap_page_queue* queue,
                         game_heap_page*       page) {

    auto* node = acquire_queue_node();

    node->value = page;
    node->next  = 0;

    for (;;) {
        auto  tail      = (u64)queue->tail;
        auto* tail_node = (game_heap_page_queue_node*)tagged_pointer(tail);
        auto  next      = (u64)tail_node->next;

        if (tail != (u64)queue->tail)
            continue;

        /*
            tail is lagging;
            help advance it before trying to enqueue again
        */
        if (tagged_pointer(next)) {
            compare_exchange(&queue->tail,
                             make_tagged_pointer(tagged_pointer(next),
                                                 tagged_sequence(tail) + 1),
                             tail);
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
            compare_exchange(&queue->tail,
                             make_tagged_pointer(node,
                                                 tagged_sequence(tail) + 1),
                             tail);
            return;
        }
    }
}

/*
    bitmapped page allocation
*/

static void* allocate_from_page(game_heap_page* page) {
    LONG free_count;

    for (;;) { /* atomically decrement free_count;
                  if 0, page is completely full */
        free_count = page->free_count;

        if (!free_count)
            return nullptr;

        if (InterlockedCompareExchange(&page->free_count,
                                       free_count - 1,
                                       free_count) == free_count) {
            break;
        }
    }

    u32 word_index = page->bitmap_cursor;

    for (;;) {
        u64 occupied = (u64)page->bitmap[word_index];

        // U64_MAX means all 64 bits are 1 (fully occupied), so scan for a word with space
        while (occupied == U64_MAX) {
            if (++word_index >= page->bitmap_word_count)
                word_index = 0; // wrap around

            occupied = (u64)page->bitmap[word_index];
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

        if (compare_exchange(&page->bitmap[word_index], exchange, occupied)) {
            page->bitmap_cursor = word_index; // cache this index for next time

            // calculate actual memory address based on slot index
            return page->blocks + page->block_size * (slot_in_word + (word_index << 6));
        }
    }
}

static void free_to_page(game_heap_page* page, void* allocation) {
    // figure out which bit belongs to this pointer
    u32 slot       = ((u8*)allocation - page->blocks) / page->block_size;
    u32 word_index = slot >> 6;                    // divide by 64 to find which u64 word it's in
    u64 mask       = 1ull << (63 - (slot & 0x3F)); // get pecific bit in that word

    for (;;) { // atomically clear the bit (mark as free)
        auto occupied = (u64)page->bitmap[word_index];

        if (!(occupied & mask))
            return; // already-free check

        u64 exchange = occupied & ~mask;

        if (compare_exchange(&page->bitmap[word_index], exchange, occupied))
            break;
    }

    for (;;) { // atomically increment the available slots counter
        LONG free_count = page->free_count;

        if (_InterlockedCompareExchange(&page->free_count,
                                        free_count + 1,
                                        free_count) == free_count) {
            return;
        }
    }
}

/*
    retrieves the metadata bucket for a specific allocation size;
    sizes 1-4 bytes -> index 0; sizes 5-8 bytes -> index 1
*/
static game_heap_size_class* get_size_class(game_small_block_heap* heap, u32 size) {
    u32 clamped_size = size < 4 ? 4 : size;
    u32 index        = ((clamped_size + 3) >> 2) - 1;

    return &heap->size_classes[index];
}

// allocate memory to store the administrative metadata for a new page
static game_heap_page* acquire_page_metadata(game_small_block_heap* heap,
                                             game_heap_size_class*  size_class,
                                             u32                    bitmap_word_count) {
    
    // check if we have recycled metadata available first
    auto* page = size_class->unused_pages;

    if (page) {
        size_class->unused_pages = (game_heap_page*)page->block_size;
        
        return page;
    }

    // otherwise allocate a new slab of metadata space
    u32 metadata_size = 0x28 + bitmap_word_count * sizeof(i64);
    u32 aligned_size  = (metadata_size + 7) & ~7u;

    game_heap_metadata_slab* slab = heap->metadata_slabs;

    if (!slab || slab->used + aligned_size >= 0x4000) {
        slab = (game_heap_metadata_slab*)
            allocate_game_heap_storage(0x4008, 0);

        slab->previous = heap->metadata_slabs;
        slab->used     = (u32)slab & 7 ? 8 - ((u32)slab & 7) : 0;

        heap->metadata_slabs = slab;
    }

    page = (game_heap_page*)(slab->storage + slab->used);
    
    slab->used += aligned_size;
    
    return page;
}

/*
    Radix Tree;
    maps arbitrary raw memory pointers back to their owning `game_heap_page`,
    required so `free_small_block()` knows where metadata is without storing headers in front of allocations
    ...which would waste cache space
*/

static void register_page(game_heap_page* page) {
    auto  address     = (u32)page->blocks;
    u32*  directories = &game_heap_page_directories.get();
    u32  &directory   = directories[address >> 24];

    if (!directory) {
        directory = (u32)allocate_game_heap_storage(0x4000, 0);
        
        std::memset((void*)directory, 0, 0x4000);
    }

    // middle 12 bits ( 0x1000 aligned page addresses )
    *(game_heap_page**)(directory + 4 * ((address >> 12) & 0x0FFF)) = page;
}

static game_heap_page* create_page(game_small_block_heap* heap, u32 block_size) {
    acquire_page_lock();

    /*
        game allocates 0x40000 chunks from the OS;
        then manually partitions them into 0x1000 pages to feed this system...
    */
    u8* blocks = game_heap_free_pages.read();

    if (blocks)
        game_heap_free_pages.write(*(u8**)blocks);
    else {
        blocks = (u8*)allocate_game_heap_storage(0x40000, 0x1000);

        u8* free_page = game_heap_free_pages.read();

        // slice a 0x40000 chunk into 0x1000 pages and link them via their first word
        for (u32 offset = 0x1000; offset < 0x40000; offset += 0x1000) {
            u8* page_blocks = blocks + offset;

            *(u8**)page_blocks = free_page;
            
            free_page = page_blocks;
        }

        game_heap_free_pages.write(free_page);
    }

    auto* size_class = get_size_class(heap, block_size);

    u32 block_count            = 0x1000 / block_size;
    u32 full_bitmap_word_count = block_count >> 6;
    u32 partial_bitmap_bits    = block_count & 0x3F;
    u32 bitmap_word_count      = full_bitmap_word_count + (partial_bitmap_bits ? 1 : 0);

    auto* page = acquire_page_metadata(heap, size_class, bitmap_word_count);

    page->block_size        = block_size;
    page->block_count       = block_count;
    page->bitmap_word_count = full_bitmap_word_count;
    page->blocks            = blocks;
    page->owner             = heap;
    page->bitmap_cursor     = 0;
    page->free_count        = block_count;
    page->state             = 2; // active (currently being allocated from)

    std::memset((void*)page->bitmap, 0, full_bitmap_word_count * sizeof(i64));

    // if the 0x1000 space doesn't perfectly divide by the block size, mark the trailing "garbage" bits as permanently occupied so we don't allocate them
    if (partial_bitmap_bits) {
        page->bitmap[full_bitmap_word_count] = (1ull << (64 - partial_bitmap_bits)) - 1;

        ++page->bitmap_word_count;
    }

    register_page(page);
    release_page_lock();
    
    return page;
}

static void release_page(game_small_block_heap* heap,
                         game_heap_page*        page) {

    acquire_page_lock();

    // 1. remove from directory mappings

    auto  address     = (u32)page->blocks;
    u32*  directories = &game_heap_page_directories.get();
    u32   directory   = directories[address >> 24];

    *(game_heap_page**)(directory + 4 * ((address >> 12) & 0x0FFF)) = nullptr;

    // 2. return 4KiB (0x1000) to the pool

    *(u8**)page->blocks = game_heap_free_pages.read();
    
    game_heap_free_pages.write(page->blocks);

    // 3. return metadata struct to the size class pool
    
    auto* size_class = get_size_class(heap, page->block_size);
    page->block_size = (u32)size_class->unused_pages;
    
    size_class->unused_pages = page;

    release_page_lock();
}

static game_heap_page* find_page(void* allocation) {
    // traverse the directory radix tree to find the metadata page for this pointer
    auto address     = (u32)allocation;
    u32* directories = &game_heap_page_directories.get();
    u32  directory   = directories[address >> 24];

    if (!directory)
        return nullptr;

    return *(game_heap_page**)(directory + 4 * ((address >> 12) & 0x0FFF));
}

/*
    ~500 lines later, the actual allocator API
*/

void* memory::game_heap::allocate(u32 size) {
    void* allocation = allocate_small_block(size);

    if (allocation)
        return allocation;

    return allocate_game_heap_storage(size, 0);
}

void memory::game_heap::free(void* allocation) {
    if (!allocation)
        return;

    if (find_page(allocation)) {
        free_small_block(allocation);

        return;
    }

    free_game_heap_storage(allocation);
}

void* memory::game_heap::allocate_small_block(u32 size) {
    game_heap_state*       state = game_heap_default.read();
    game_small_block_heap* heap  = state->small_block_heap;

    if (!heap || size > 0x100) // only sizes up to 256 bytes
        return nullptr;

    game_heap_size_class* size_class = get_size_class(heap, size);

    for (;;) {
        game_heap_page* page = size_class->active_page;

        // fast path: we have a designated active page for this size clas
        if (page) {
            void* allocation = allocate_from_page(page);

            if (allocation) {
                state->small_block_bytes += size_class->block_size;
                
                return allocation;
            }

            /*
                page is full...
                transition state active (2) to inactive (full) (1), then remove it as the active_page so we fetch a new one next loop
            */
            if (_InterlockedCompareExchange(&page->state, 1, 2) == 2)
                _InterlockedCompareExchange((volatile LONG*)&size_class->active_page,
                                            0,
                                            (LONG)page);

            continue;
        }

        // slooow path: no active page, pull a partially full one from the queue
        if (!dequeue_page(&size_class->available_pages, &page))
        {   
            // even sloooooower path: queue is empty, create a brand new page
            page = create_page(heap, size_class->block_size);

            // attempt to install as active page; if another thread beat us, release and try again
            if (_InterlockedCompareExchange((volatile LONG*)&size_class->active_page, (LONG)page, 0))
                release_page(heap, page);

            continue;
        }

        void* allocation = allocate_from_page(page);

        if (!allocation) {
            page->state = 1; // it was in the queue, but is actually full, so mark inactive
            
            continue;
        }

        page->state = 2; // we succesfully pulled from it, mark active

        // make it the primary active page for this size class
        if (_InterlockedCompareExchange((volatile LONG*)&size_class->active_page, (LONG)page, 0)) {
            // another thread set an active page first, transition to queued (3) and push it back onto the queue
            page->state = 3;
            
            enqueue_page(&size_class->available_pages, page);
        }

        state->small_block_bytes += size_class->block_size;
        
        return allocation;
    }
}

void memory::game_heap::free_small_block(void* allocation) {
    if (!allocation)
        return;

    game_heap_state* state = game_heap_default.read();
    game_heap_page*  page  = find_page(allocation);

    state->small_block_bytes -= page->block_size;
    
    free_to_page(page, allocation);

    game_heap_size_class* size_class =
        get_size_class(state->small_block_heap, page->block_size);

    // if the page was full (1), it now has space; tranisition it to queued (3) and queue it up
    if (_InterlockedCompareExchange(&page->state, 3, 1) == 1)
        enqueue_page(&size_class->available_pages, page);

    // if the page is not in the queue or still has active allocations, we're done
    if (page->state != 3 || page->free_count != (LONG)page->block_count)
        return;

    /*
        we should give this 4KiB (0x1000) block back to the main heap to prevent memory hoarding;
        because it's in a queue, we have to cycle the queue until we find it...
    */
    for (u32 scan_count = 0; scan_count < 0x100; ++scan_count) {
        game_heap_page* queued_page;

        if (!dequeue_page(&size_class->available_pages, &queued_page))
            return; // queue is empty, abort

        if (queued_page != page) {
            // this isn't our page, put it back
            enqueue_page(&size_class->available_pages, queued_page);
            
            continue;
        }

        // hello there, let's check if it's empty
        if (queued_page->state == 3 && queued_page->free_count == (LONG)queued_page->block_count)
            // it is, bye bye
            release_page(state->small_block_heap, queued_page);
        else
            // another thread allocated from it while we were looking for it, put it back
            enqueue_page(&size_class->available_pages, queued_page);

        return;
    }
}
