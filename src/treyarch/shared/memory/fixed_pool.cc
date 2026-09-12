#include "retail.hh"
#include "treyarch/shared/memory/fixed_pool.hh"

namespace treyarch { namespace memory {
    class fixed_pool_allocator {

    public:
        virtual ~fixed_pool_allocator() = default;
        virtual void* allocate(u32 size) = 0;
        virtual void  free(void* allocation) = 0;
    };
}} // treyarch::memory

using namespace treyarch;
using namespace treyarch::memory;

void fixed_pool::initialize(const char* pool_name,
                            u32         requested_slot_size,
                            u32         requested_alignment,
                            u32         requested_slots_per_block,
                            u32         requested_initial_block_count) {

    name                = pool_name;
    slot_size           = requested_slot_size;
    alignment           = requested_alignment;
    slots_per_block     = requested_slots_per_block;
    allocation_count    = 0;
    growable            = 1;
    allocator           = (fixed_pool_allocator*)retail::sub_6869E0();
    block_count         = 0;
    initial_block_count = requested_initial_block_count;
    free_list           = nullptr;
    initialized         = true;

    u32 quotient = slot_size / alignment;

    if (slot_size & (alignment - 1))
        ++quotient;

    slot_size = quotient * alignment;

    add_blocks(initial_block_count);

    lock.owner    = 0;
    lock.state    = 0;
    lock.depth    = 0;
    lock.reserved = 0;
}

void fixed_pool::add_blocks(u32 count) {
    while (count--) {
        u32 allocation_size = alignment + slots_per_block * slot_size + sizeof(void*);
        u8* block           = (u8*)allocator->allocate(allocation_size);

        *(void**)block = block_list;
        block_list     = block;

        u8* first_slot = (u8*)(((u32)(block + sizeof(void*)) & ~(alignment - 1)) + alignment);
        u8* slot       = first_slot;

        for (u32 index = 1; index < slots_per_block; ++index) {
            u8* next = slot + slot_size;

            *(void**)slot = next;
            slot          = next;
        }

        *(void**)slot = free_list;
        free_list     = first_slot;

        ++block_count;
    }
}

void* fixed_pool::allocate() {
    engine_lock_scope scope(&lock);

    if (!free_list && growable)
        add_blocks(1);

    void* allocation = free_list;

    if (allocation)
        free_list = *(void**)allocation;

    ++allocation_count;

    return allocation;
}

void fixed_pool::release(void* allocation) {
    engine_lock_scope scope(&lock);

    *(void**)allocation = free_list;
    free_list           = allocation;

    --allocation_count;
}
