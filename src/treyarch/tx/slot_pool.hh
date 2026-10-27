#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace tx {
    // embedded in each pooled record.
    // ids carry a generation above the index bits, the top bit is set while the record is free
    struct slot_entry {
        slot_entry* next;
        slot_entry* prev;
        i32         id;
    };

    struct slot_pool {
        slot_entry* entries;
        slot_entry  free_list;
        slot_entry  used_list;
        u32         stride;
        i32         count;
        u32         mask;

        slot_entry* entry_at(u32 index) const {
            return (slot_entry*)((u8*)entries + stride * index);
        }
    };

    bool slot_pool_init(slot_pool* pool, slot_entry* entries, i32 count, u32 stride);

    inline i32 slot_first(const slot_pool* pool) {
        return pool->used_list.next->id;
    }

    // inlined @ sub_A15530
    inline i32 slot_new(slot_pool* pool) {
        slot_entry* entry = pool->free_list.next;

        if (entry == &pool->free_list)
            return -1;

        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;

        entry->next = &pool->used_list;
        entry->prev = pool->used_list.prev;
        pool->used_list.prev->next = entry;
        pool->used_list.prev       = entry;

        entry->id = (entry->id + pool->mask + 1) & 0x7FFFFFFF;

        // a fresh id never equals its bare index
        if (!(~(pool->mask | 0x80000000) & entry->id))
            entry->id += pool->mask + 1;

        return entry->id;
    }

    // inlined @ sub_A15D00
    inline void slot_free(slot_pool* pool, i32 id) {
        if (id & 0x80000000 || (id & pool->mask) >= (u32)pool->count)
            return;

        slot_entry* entry = pool->entry_at(id & pool->mask);

        if (entry->id != id)
            return;

        entry->id |= 0x80000000;

        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;

        entry->next = &pool->free_list;
        entry->prev = pool->free_list.prev;
        pool->free_list.prev->next = entry;
        pool->free_list.prev       = entry;
    }

    // inlined @ sub_A15530
    inline i32 slot_index(const slot_pool* pool, i32 id) {
        if (id < 0 || (id & pool->mask) >= (u32)pool->count)
            return -1;

        const i32 entry_id = pool->entry_at(id & pool->mask)->id;

        if (entry_id != id)
            return -1;

        return entry_id & pool->mask;
    }

    // inlined @ sub_A173E0
    inline i32 slot_next(const slot_pool* pool, i32 id) {
        if (id < 0 || (id & pool->mask) >= (u32)pool->count)
            return -1;

        slot_entry* entry = pool->entry_at(id & pool->mask);

        return entry->id == id ? entry->next->id : -1;
    }

    ASSERT_SIZEOF  (slot_entry,     0x0C);
    ASSERT_OFFSETOF(slot_entry, id, 0x08);

    ASSERT_SIZEOF  (slot_pool,            0x28);
    ASSERT_OFFSETOF(slot_pool, free_list, 0x04);
    ASSERT_OFFSETOF(slot_pool, used_list, 0x10);
    ASSERT_OFFSETOF(slot_pool, stride,    0x1C);
    ASSERT_OFFSETOF(slot_pool, count,     0x20);
    ASSERT_OFFSETOF(slot_pool, mask,      0x24);
}} // treyarch::tx
