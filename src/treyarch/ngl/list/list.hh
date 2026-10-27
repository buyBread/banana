#pragma once

#include "treyarch/ngl/list/render_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    struct render_sort_entry {
        render_node*         node;
        render_node_sort_key sort_key;
    };

    struct opaque_compare {
        bool operator()(const render_sort_entry &first, const render_sort_entry &second) const;
    };

    struct trans_compare {
        bool operator()(const render_sort_entry &first, const render_sort_entry &second) const;
    };

    void* list_alloc(u32 size, u32 alignment);
    void  replace_list_work_buffer(u32 size);

    scene* ngli_list_init();
    void   ngli_list_send();

    template <typename compare_type>
    void sort_list(render_node* &head, u32 count, compare_type compare);

    void begin_render_node(render_node* head);
    void advance_render_node();
    void render_nodes(render_node* head);

    namespace references {
        inline util::memory_reference<render_node*> cur_render_node  { 0x01115C5C };
        inline util::memory_reference<render_node*> prev_render_node { 0x01115C60 };
        inline util::memory_reference<u8*>          list_work        { 0x01115C64 };
        inline util::memory_reference<u32>          list_work_size   { 0x01115C68 };
        inline util::memory_reference<u8*>          list_work_pos    { 0x01115C6C };
    } // references

    ASSERT_SIZEOF  (render_sort_entry,           0x08);
    ASSERT_OFFSETOF(render_sort_entry, node,     0x00);
    ASSERT_OFFSETOF(render_sort_entry, sort_key, 0x04);
}} // treyarch::ngl
