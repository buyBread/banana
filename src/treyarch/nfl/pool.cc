#include "treyarch/nfl/pool.hh"

using namespace treyarch;

// sub_A17F10
bool treyarch::nfl::initialize_pool(pool* pool, pool_node* nodes, i32 count, u32 stride) {
    if (count <= 0 || (u32)count > 0xFFFFF || stride < sizeof(pool_node))
        return false;

    // the mask covers every bit of the count, so 64 records get a mask of 127
    u32 bits = 0;

    for (i32 remaining = count; remaining; remaining >>= 1)
        ++bits;

    pool->nodes  = nodes;
    pool->stride = stride;
    pool->count  = count;
    pool->mask   = (1 << bits) - 1;

    pool->free_list.id   = -1;
    pool->free_list.next = &pool->free_list;
    pool->free_list.prev = &pool->free_list;

    pool->used_list.id   = -1;
    pool->used_list.next = &pool->used_list;
    pool->used_list.prev = &pool->used_list;

    for (i32 index = 0; index < count; ++index) {
        pool_node* node = (pool_node*)((u8*)pool->nodes + pool->stride * index);

        node->id   = index | 0x80000000;
        node->next = &pool->free_list;
        node->prev = pool->free_list.prev;

        pool->free_list.prev->next = node;
        pool->free_list.prev       = node;
    }

    return true;
}
