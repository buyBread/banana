#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/internal_programs.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/d3d9/vertex_formats.hh"
#include "treyarch/ngl/quad/quad.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

struct quad_color_draw_vertex {
    f32 x;
    f32 y;
    f32 z;
    u32 color;
};

struct quad_textured_draw_vertex {
    f32 x;
    f32 y;
    f32 z;
    u32 color;
    f32 u;
    f32 v;
};

ASSERT_SIZEOF(quad_color_draw_vertex,    0x10);
ASSERT_SIZEOF(quad_textured_draw_vertex, 0x18);

void ngl::quad_renderer::render(node* value) {
    const quad &source = value->value;

    scene* current = ngl::references::current_scene.read();

    d3d9::set_render_state(D3DRS_ZWRITEENABLE, TRUE);
    d3d9::apply_blend_mode(source.blend_mode);
    d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CCW);
    d3d9::set_render_state(D3DRS_FOGENABLE, FALSE);

    d3d9::internal_program_set &programs = d3d9::references::internal_programs.get();

    IDirect3DPixelShader9** pixel_program = source.pixel_program;

    if (source.texture_data) {
        ngl::references::animation_frame.write(current->ifl_frame);
        d3d9::bind_texture(0, source.texture_data, source.map_flags, 3);
        d3d9::set_vertex_program(programs.screen_pcuv_vertex_program);

        if (!pixel_program)
            pixel_program = &programs.texture_modulate_pixel_program;
    } else {
        d3d9::set_vertex_program(programs.screen_pc_vertex_program);

        if (!pixel_program)
            pixel_program = &programs.screen_pc_pixel_program;
    }

    f32 depth_z = source.z;

    if (current->near_plane > depth_z)
        depth_z = current->near_plane;
    else if (depth_z > current->far_plane)
        depth_z = current->far_plane;

    f32 numerator = (f32)((f64)current->view_to_screen.z.z * (f64)depth_z);
    numerator = (f32)((f64)numerator + (f64)current->view_to_screen.w.z);

    f32 denominator = (f32)((f64)current->view_to_screen.z.w * (f64)depth_z);
    denominator = (f32)((f64)denominator + (f64)current->view_to_screen.w.w);

    f32 depth = (f32)((f64)numerator / (f64)denominator);

    if (depth < 0.0f)
        depth = 0.0f;
    else if (depth > 1.0f)
        depth = 1.0f;

    d3d9::set_pixel_program(*pixel_program);

    d3d9::binding_cache &bindings = d3d9::references::bindings.get();
    bindings.stream_source      = nullptr;
    bindings.vertex_declaration = nullptr;

    auto* device = d3d9::references::device.get();

    const matrix4x4 &matrix = current->derived_matrix_2d0;

    if (source.texture_data) {
        quad_textured_draw_vertex vertices[4];

        for (u32 index = 0; index < 4; ++index) {
            const quad_vertex &input = source.vertices[index];

            quad_textured_draw_vertex &output = vertices[index];

            f32 x = (f32)((f64)input.x * (f64)matrix.x.x);
            f32 y = (f32)((f64)input.y * (f64)matrix.y.y);

            output.x     = (f32)((f64)x + (f64)matrix.w.x);
            output.y     = (f32)((f64)y + (f64)matrix.w.y);
            output.z     = depth;
            output.color = input.color;
            output.u     = input.u;
            output.v     = input.v;
        }

        device->SetVertexDeclaration(d3d9::references::quad_position_color_uv_format.get().declaration);
        device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(quad_textured_draw_vertex));
    } else {
        quad_color_draw_vertex vertices[4];

        for (u32 index = 0; index < 4; ++index) {
            const quad_vertex &input = source.vertices[index];
            quad_color_draw_vertex &output = vertices[index];

            f32 x = (f32)((f64)input.x * (f64)matrix.x.x);
            f32 y = (f32)((f64)input.y * (f64)matrix.y.y);

            output.x = (f32)((f64)x + (f64)matrix.w.x);
            output.y = (f32)((f64)y + (f64)matrix.w.y);
            output.z = depth;
            output.color = input.color;
        }

        device->SetVertexDeclaration(d3d9::references::quad_position_color_format.get().declaration);
        device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(quad_color_draw_vertex));
    }
}
