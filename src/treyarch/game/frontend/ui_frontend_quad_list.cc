#include "treyarch/game/frontend/ui_frontend.hh"
#include "treyarch/ngl/d3d9/scratch_mesh.hh"
#include "treyarch/ngl/mesh/submission.hh"
#include "treyarch/ngl/mesh/transient.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/shaders/program_exports.hh"

using namespace treyarch;

struct quad_scratch_vertex {
    i16 x;
    i16 y;
    i16 z;
    i16 pad;
    u32 color;
    i16 u;
    i16 v;
};

ASSERT_SIZEOF(quad_scratch_vertex, 0x10);

// sub_6CC9F0
void ui_frontend::draw_quad_list() {
    if (!quad_list.size)
        return;

    ngl::list_begin_scene(ngl::scene_parameter_defaults);
    ngl::set_scene_name("UIFrontEnd::DrawQuadList");
    ngl::set_clear_flags(0);

    matrix4x4 identity;
    identity.identity();
    ngl::set_world_to_view_matrix(&identity);

    ngl::set_z_test_enable(false);
    ngl::set_z_write_enable(false);

    container::legacy_list_node<ngl::quad*>* head = quad_list.head;

    for (container::legacy_list_node<ngl::quad*>* node = head->next; node != head; node = node->next) {
        ngl::quad* value = node->value;

        if (!value || !flat_material)
            continue;

        flat_material->texture_data = value->texture_data ?
            value->texture_data : ngl::references::white_texture.read();
        flat_material->blend_mode_low  = (u32)value->blend_mode;
        flat_material->blend_mode_high = (u32)(value->blend_mode >> 32);
        flat_material->map_flags       = value->map_flags;

        ngl::mesh* mesh = ngl::create_transient_mesh(0x40000, 1);
        ngl::mesh_section* section = ngl::d3d9::allocate_scratch_mesh_section(
            D3DPT_TRIANGLESTRIP, 4, 4, &ngl::shaders::program_exports::pcuv::format.get());

        ngl::attach_transient_mesh_section(mesh, section, &flat_material->base, 1);

        u16* indices = ngl::d3d9::lock_scratch_mesh_indices(section);
        auto* vertices = (quad_scratch_vertex*)ngl::d3d9::lock_scratch_mesh_vertices(section);

        for (u32 index = 0; index < 4; ++index) {
            const ngl::quad_vertex &source = value->vertices[index];

            f32 x = source.x * 0.009999999776482582f;
            f32 y = source.y * 0.009999999776482582f * -1.0f;

            vertices[index].x     = (i16)(i32)(x * 100.0f);
            vertices[index].y     = (i16)(i32)(y * 100.0f);
            vertices[index].z     = 0;
            vertices[index].color = source.color;
            vertices[index].u     = (i16)(i32)(source.u * 1024.0f);
            vertices[index].v     = (i16)(i32)(source.v * 1024.0f);
            indices[index]        = (u16)index;
        }

        ngl::d3d9::unlock_scratch_mesh_indices(section);
        ngl::d3d9::unlock_scratch_mesh_vertices(section);

        matrix4x4 transform = quad_transform;
        ngl::list_add_transient_mesh(mesh, &transform);
    }

    ngl::list_end_scene();
}

// sub_6CCE50
void ui_frontend::clear_quad_list() {
    quad_list_state_200 = 0;

    container::clear_legacy_list(&quad_list);
}
