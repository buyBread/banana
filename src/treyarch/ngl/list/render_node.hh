#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    union render_node_sort_key {
        u32 integer;
        f32 floating;
    };

    struct render_node {
        void*                vtable;
        render_node*         next;
        render_node_sort_key sort_key;

        void render();
    };

    struct render_sort_entry {
        render_node*         node;
        render_node_sort_key sort_key;
    };

    ASSERT_SIZEOF  (render_node_sort_key,  0x04);
    ASSERT_SIZEOF  (render_node,           0x0C);
    ASSERT_OFFSETOF(render_node, next,     0x04);
    ASSERT_OFFSETOF(render_node, sort_key, 0x08);

    ASSERT_SIZEOF  (render_sort_entry,           0x08);
    ASSERT_OFFSETOF(render_sort_entry, node,     0x00);
    ASSERT_OFFSETOF(render_sort_entry, sort_key, 0x04);
}} // treyarch::ngl
