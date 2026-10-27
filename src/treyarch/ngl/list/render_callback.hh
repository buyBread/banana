#pragma once

#include "treyarch/ngl/list/render_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    enum e_node_type : u32 {
        node_scene  = 0,
        node_custom = 1
    };

    using custom_node_fn = void (__cdecl*)(void* data);

    struct render_callback_node : render_node {
        e_node_type    type;
        custom_node_fn fn;
        void*          data;
        ngl::sort_info sort_info;

        void render() override;
        void get_desc(char* desc) override;
        void get_sort_info(ngl::sort_info &info) override;
    };

    scene* list_add_custom_node(custom_node_fn fn, void* data, const ngl::sort_info* sort_info);

    namespace references {
        inline util::memory_reference<void*> render_callback_node_vftable { 0x00DB716C };
    } // references

    ASSERT_SIZEOF  (render_callback_node,            0x20);
    ASSERT_OFFSETOF(render_callback_node, type,      0x0C);
    ASSERT_OFFSETOF(render_callback_node, fn,        0x10);
    ASSERT_OFFSETOF(render_callback_node, data,      0x14);
    ASSERT_OFFSETOF(render_callback_node, sort_info, 0x18);
}} // treyarch::ngl
