#pragma once

#include "treyarch/ngl/list/render_node.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace render_callback {
    using function = void (__cdecl*)(void* data);

    struct sort_info {
        u32                  type;
        render_node_sort_key key;
    };

    struct node {
        render_node base;
        u32         type;
        function    callback;
        void*       data;
        sort_info   sorting;
    };

    void render(node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00DB716C };
    } // references

    ASSERT_SIZEOF  (sort_info,       0x08);
    ASSERT_OFFSETOF(sort_info, type, 0x00);
    ASSERT_OFFSETOF(sort_info, key,  0x04);

    ASSERT_SIZEOF  (node,           0x20);
    ASSERT_OFFSETOF(node, base,     0x00);
    ASSERT_OFFSETOF(node, type,     0x0C);
    ASSERT_OFFSETOF(node, callback, 0x10);
    ASSERT_OFFSETOF(node, data,     0x14);
    ASSERT_OFFSETOF(node, sorting,  0x18);
}}} // treyarch::ngl::render_callback
