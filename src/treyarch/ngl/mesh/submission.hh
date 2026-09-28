#pragma once

#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    struct mesh_node : fx::mesh_node_data {
        u8 reserved_0a4[0x0C];
    };

    i32        cull_mesh_sphere(const vector4 &center, f32 radius, u8 flags);
    mesh_node* create_transient_mesh_node(mesh* value, const matrix4x4* local_to_world);
    mesh_node* list_add_transient_mesh(mesh* value, const matrix4x4* local_to_world);

    namespace references {
        inline util::memory_reference<u8>                default_node_info  { 0x011161A0 };
        inline util::memory_reference<scene_parameters*> default_parameters { 0x00F52804 };
    } // references

    ASSERT_SIZEOF(mesh_node, 0xB0);
}} // treyarch::ngl
