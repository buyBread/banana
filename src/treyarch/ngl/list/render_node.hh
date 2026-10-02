#pragma once

#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl {
    union render_node_sort_key {
        u32 integer;
        f32 floating;
    };

    enum e_sort_type : u32 {
        sort_opaque      = 0,
        sort_translucent = 1
    };

    struct sort_info {
        e_sort_type          type;
        render_node_sort_key key;
    };

    struct scene;

    struct render_node {
        void*                vtable;
        render_node*         next;
        render_node_sort_key sort_key;

        void render();
        void get_sort_info(sort_info* result); // vtable slot 4
    };

    scene* list_add_node(render_node* value);

    struct render_sort_entry {
        render_node*         node;
        render_node_sort_key sort_key;
    };

    ASSERT_SIZEOF  (render_node_sort_key,  0x04);
    ASSERT_SIZEOF  (sort_info,       0x08);
    ASSERT_OFFSETOF(sort_info, type, 0x00);
    ASSERT_OFFSETOF(sort_info, key,  0x04);

    ASSERT_SIZEOF  (render_node,           0x0C);
    ASSERT_OFFSETOF(render_node, next,     0x04);
    ASSERT_OFFSETOF(render_node, sort_key, 0x08);

    ASSERT_SIZEOF  (render_sort_entry,           0x08);
    ASSERT_OFFSETOF(render_sort_entry, node,     0x00);
    ASSERT_OFFSETOF(render_sort_entry, sort_key, 0x04);
}} // treyarch::ngl
