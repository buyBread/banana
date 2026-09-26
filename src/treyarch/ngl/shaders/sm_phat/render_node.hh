#pragma once

#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace shaders { namespace sm_phat {
    struct render_node {
        ngl::render_node                    base;
        fx::mesh_node_data*                 node_data;
        mesh_section*                       section;
        generated_material::material_data*  material_data;
        u8                                  queue_class;
        u8                                  reserved_019[3];
        generated_material::scene_snapshot* scene_snapshot;
    };

    void render       (render_node* value);
    void render_normal(render_node* value);
    void render_shadow(render_node* value);

    namespace references {
        inline util::memory_reference<void*> node_vtable { 0x00BE55B8 };
    } // references

    ASSERT_SIZEOF  (render_node,                 0x20);
    ASSERT_OFFSETOF(render_node, node_data,      0x0C);
    ASSERT_OFFSETOF(render_node, section,        0x10);
    ASSERT_OFFSETOF(render_node, material_data,  0x14);
    ASSERT_OFFSETOF(render_node, queue_class,    0x18);
    ASSERT_OFFSETOF(render_node, scene_snapshot, 0x1C);
}}}} // treyarch::ngl::shaders::sm_phat
