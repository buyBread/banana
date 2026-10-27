#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/material.hh"
#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl { namespace fx {
    struct shader_node : ngl::shader_node {
        material* material_data;
        effect*   effect_data;
        i32       technique_index;
        u32       queue_class;
        u32       order_key;

        void render() override;
        i32  get_list_type() override;

        effect* select_effect();
    };

    namespace references {
        inline util::memory_reference<void*> shader_node_vftable { 0x00DB7B84 };
    } // references

    ASSERT_SIZEOF  (shader_node,                  0x28);
    ASSERT_OFFSETOF(shader_node, material_data,   0x14);
    ASSERT_OFFSETOF(shader_node, effect_data,     0x18);
    ASSERT_OFFSETOF(shader_node, technique_index, 0x1C);
    ASSERT_OFFSETOF(shader_node, queue_class,     0x20);
    ASSERT_OFFSETOF(shader_node, order_key,       0x24);
}}} // treyarch::ngl::fx
