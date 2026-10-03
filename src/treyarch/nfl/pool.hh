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
