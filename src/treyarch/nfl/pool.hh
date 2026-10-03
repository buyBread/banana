#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace nfl {
    // embedded in each pooled record.
    // ids carry a generation above the index bits, the top bit is set while the record is free
    struct pool_node {
        pool_node* next;
        pool_node* prev;
        i32        id;
    };

    struct pool {
        pool_node* nodes;
        pool_node  free_list;
        pool_node  used_list;
        u32        stride;
        i32        count;
        u32        mask;

        pool_node* node_at(u32 index) const {
            return (pool_node*)((u8*)nodes + stride * index);
        }

        i32 first_id() const {
            return used_list.next->id;
        }

        // inlined @ sub_A15530
        i32 allocate() {
            pool_node* node = free_list.next;

            if (node == &free_list)
                return -1;

            node->prev->next = node->next;
            node->next->prev = node->prev;

            node->next = &used_list;
            node->prev = used_list.prev;
            used_list.prev->next = node;
            used_list.prev       = node;

            node->id = (node->id + mask + 1) & 0x7FFFFFFF;

            // a fresh id never equals its bare index
            if (!(~(mask | 0x80000000) & node->id))
                node->id += mask + 1;

            return node->id;
        }

        // inlined @ sub_A15D00
        void release(i32 id) {
            if (id & 0x80000000 || (id & mask) >= (u32)count)
                return;

            pool_node* node = node_at(id & mask);

            if (node->id != id)
                return;

            node->id |= 0x80000000;

            node->prev->next = node->next;
            node->next->prev = node->prev;

            node->next = &free_list;
            node->prev = free_list.prev;
            free_list.prev->next = node;
            free_list.prev       = node;
        }

        // inlined @ sub_A15530
        i32 find(i32 id) const {
            if (id < 0 || (id & mask) >= (u32)count)
                return -1;

            const i32 node_id = node_at(id & mask)->id;

            if (node_id != id)
                return -1;

            return node_id & mask;
        }

        // inlined @ sub_A173E0
        i32 next_id(i32 id) const {
            if (id < 0 || (id & mask) >= (u32)count)
                return -1;

            pool_node* node = node_at(id & mask);

            return node->id == id ? node->next->id : -1;
        }
    };

    bool initialize_pool(pool* pool, pool_node* nodes, i32 count, u32 stride);

    ASSERT_SIZEOF  (pool_node,     0x0C);
    ASSERT_OFFSETOF(pool_node, id, 0x08);

    ASSERT_SIZEOF  (pool,            0x28);
    ASSERT_OFFSETOF(pool, free_list, 0x04);
    ASSERT_OFFSETOF(pool, used_list, 0x10);
    ASSERT_OFFSETOF(pool, stride,    0x1C);
    ASSERT_OFFSETOF(pool, count,     0x20);
    ASSERT_OFFSETOF(pool, mask,      0x24);
}} // treyarch::nfl
