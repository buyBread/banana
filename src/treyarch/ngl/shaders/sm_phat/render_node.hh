#pragma once

#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
    struct node : shader_node {
        generated_material::material_data*  material_data;
        u8                                  queue_class;
        u8                                  reserved_019[3];
        generated_material::scene_snapshot* scene_snapshot;

        void render() override;

        void render_normal();
        void render_shadow();
    };

    namespace references {
        inline util::memory_reference<void*> node_vftable { 0x00BE55B8 };
    } // references

    ASSERT_SIZEOF  (node,                 0x20);
    ASSERT_OFFSETOF(node, material_data,  0x14);
    ASSERT_OFFSETOF(node, queue_class,    0x18);
    ASSERT_OFFSETOF(node, scene_snapshot, 0x1C);
}}}} // treyarch::ngl::shaders::sm_phat
