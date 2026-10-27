#include "treyarch/tx/slot_pool.hh"

using namespace treyarch;

// sub_A17F10
bool treyarch::tx::slot_pool_init(slot_pool* pool, slot_entry* entries, i32 count, u32 stride) {
    if (count <= 0 || (u32)count > 0xFFFFF || stride < sizeof(slot_entry))
        return false;

    // the mask covers every bit of the count, so 64 records get a mask of 127
    u32 bits = 0;

    for (i32 remaining = count; remaining; remaining >>= 1)
        ++bits;

    pool->entries = entries;
    pool->stride  = stride;
    pool->count   = count;
    pool->mask    = (1 << bits) - 1;

    pool->free_list.id   = -1;
    pool->free_list.next = &pool->free_list;
    pool->free_list.prev = &pool->free_list;

    pool->used_list.id   = -1;
    pool->used_list.next = &pool->used_list;
    pool->used_list.prev = &pool->used_list;

    for (i32 index = 0; index < count; ++index) {
        slot_entry* entry = (slot_entry*)((u8*)pool->entries + pool->stride * index);

        entry->id   = index | 0x80000000;
        entry->next = &pool->free_list;
        entry->prev = pool->free_list.prev;

        pool->free_list.prev->next = entry;
        pool->free_list.prev       = entry;
    }

    return true;
}
