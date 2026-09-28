#include <bit>
#include <cmath>
#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/mesh_submission.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/fx/mesh_node_data.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/mesh/mesh.hh"
#include "treyarch/ngl/scene/parameters.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shadow/device_resources.hh"
#include "treyarch/ngl/shadow/projection.hh"

using namespace treyarch;

namespace treyarch { namespace ngl { namespace shaders { namespace generated_material {
    void set_pixel_constant(      i32      register_index,
                            const vector4* values,
                                  u32      count = 1) {

        if (register_index == -1)
            return;

        d3d9::references::device.get()
            ->SetPixelShaderConstantF(register_index,
                                      (const f32*)values,
                                      count);
    }

    void bind_texture(u32 stage, texture* value) {
        if (value)
            d3d9::set_texture(stage, value->gpu_texture.resource);
    }

    i32 binding(const program_exports::pixel_pipeline_descriptor &pipeline,
                u32                                               dword_index) {

        return pipeline.bindings[dword_index - 2];
    }
}}}} // treyarch::ngl::shaders::generated_material

bool ngl::shaders::generated_material::texture_is_usable(const texture* value) {
    return value &&
           value != ngl::references::white_texture.read() &&
           value->gpu_texture.width * value->gpu_texture.height >= 256;
}

void ngl::shaders::generated_material::prepare_scene_snapshot(      scene_snapshot*     snapshot,
                                                              const material_data*      material,
                                                              const fx::mesh_node_data* node_data,
                                                              const mesh_section*       section) {

    std::memcpy(snapshot,
                &references::default_scene_snapshot.get(),
                sizeof(*snapshot));

    snapshot->local_to_world = node_data->local_to_world;
    snapshot->animation_time = references::animation_time.read();

    if (node_data->node_info && (node_data->node_info[0] & 2)) {
        const vector3 &scale = *(const vector3*)(node_data->node_info + 0x10);

        snapshot->base_transform = matrix4x4(scale.x, 0.0f,   0.0f,   0.0f,
                                             0.0f,   scale.y, 0.0f,   0.0f,
                                             0.0f,   0.0f,   scale.z, 0.0f,
                                             0.0f,   0.0f,   0.0f,   1.0f);
    }

    u32 seed = (u32)section;
    seed ^= seed >> 11;
    seed ^= (seed & 0xFF3A58ADu) << 7;
    seed ^= (seed & 0xFFFFDF8Cu) << 15;
    snapshot->random_seed = seed;

    scene_parameters* parameters = node_data->parameters;

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_random_seed.read())) {

        u32 parameter_id = fx::references::parameter_id_material_random_seed.read();
        u32 value = (u32)get_scene_parameter(parameters, parameter_id);

        value ^= value >> 11;
        value ^= (value & 0xFF3A58ADu) << 7;
        value ^= (value & 0xFFFFDF8Cu) << 15;

        snapshot->random_seed = value;
    }

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_unknown_178.read())) {

        u32 parameter_id = fx::references::parameter_id_material_unknown_178.read();
        snapshot->unknown_178 = (u32)get_scene_parameter(parameters, parameter_id);
    }

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_scalar.read())) {

        u32 parameter_id = fx::references::parameter_id_material_scalar.read();
        snapshot->scalar_17c = std::bit_cast<f32>((u32)get_scene_parameter(parameters, parameter_id));
    }

    snapshot->scalar_180 = snapshot->scalar_17c;

    f32 animation_time = ngl::references::current_scene.read()->current_animation_time;

    f32 scale_u  = material->texture_scale_u;
    f32 scale_v  = material->texture_scale_v;
    f32 offset_u = material->texture_offset_u + material->texture_scroll_u * animation_time;
    f32 offset_v = material->texture_offset_v + material->texture_scroll_v * animation_time;

    if (material->texture_wrap_u != 0.0f) {
        scale_u /= material->texture_wrap_u;
        f32 wrapped = offset_u * material->texture_wrap_u;
        offset_u = (wrapped - std::floor(wrapped)) * scale_u;
    }

    if (material->texture_wrap_v != 0.0f) {
        scale_v /= material->texture_wrap_v;
        f32 wrapped = offset_v * material->texture_wrap_v;
        offset_v = (wrapped - std::floor(wrapped)) * scale_v;
    }

    snapshot->texture_matrix = matrix4x4(scale_u, 0.0f,   0.0f, 0.0f,
                                         0.0f,   scale_v, 0.0f, 0.0f,
                                         0.0f,   0.0f,   1.0f, 0.0f,
                                         offset_u, offset_v, 0.0f, 1.0f);

    const vector4 &value_0 = material->material_values[0];
    const vector4 &value_1 = material->material_values[1];
    const vector4 &value_2 = material->material_values[2];

    snapshot->material_values[0] = vector4(value_0.x, value_2.z, 1.0f, value_1.z);
    snapshot->material_values[1] = vector4(value_0.y, value_2.w, 0.0f, value_1.w);
    snapshot->material_values[2] = vector4(value_0.z, value_1.x, value_2.x, 0.0f);
    snapshot->material_values[3] = vector4(value_0.w, value_1.y, value_2.y, 0.0f);

    if (material->mode != 1) {
        snapshot->material_values[2].x = 0.0f;
        snapshot->material_values[3].x = 1.0f;
    }

    snapshot->material_vector_140 = vector4(-material->animation_scale_u,
                                            -material->animation_scale_u,
                                            1.0f - material->animation_scale_v,
                                            1.0f - material->animation_scale_v);
    snapshot->diffuse_texture = material->diffuse_texture;
    snapshot->scene_initializer_data = material->scene_initializer_data;

    scene_initializer* initializer = material->scene_initializer_data;

    if (initializer && initializer->value) {
        using apply_function = void(__thiscall*)(void*, scene_snapshot*);

        void** vtable = *(void***)initializer->value;
        ((apply_function)vtable[9])(initializer->value, snapshot); // todo
    }

    f32 material_alpha = 1.0f;

    if (has_scene_parameter(parameters, fx::references::parameter_id_tint_color.read())) {
        const vector4 &tint = *(const vector4*)get_scene_parameter(parameters,
                                                                   fx::references::parameter_id_tint_color.read());

        vector4 scale = tint;

        if (tint.x > 100.0f || tint.y > 100.0f || tint.z > 100.0f || tint.w > 100.0f) {
            scale = vector4(tint.x / 1000.0f,
                            tint.y / 1000.0f,
                            tint.z / 1000.0f,
                            1.0f);
        } else
            material_alpha = tint.w;

        matrix4x4 tint_matrix(scale.x, 0.0f,    0.0f,    0.0f,
                              0.0f,    scale.y, 0.0f,    0.0f,
                              0.0f,    0.0f,    scale.z, 0.0f,
                              0.0f,    0.0f,    0.0f,    scale.w);

        snapshot->light_matrix = snapshot->light_matrix * tint_matrix;
    }

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_alpha.read())) {

        u32 parameter_id = fx::references::parameter_id_material_alpha.read();
        f32 alpha = std::bit_cast<f32>((u32)get_scene_parameter(parameters, parameter_id));
        material_alpha *= alpha;
    }

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_light_matrix.read())) {

        u32 parameter_id = fx::references::parameter_id_material_light_matrix.read();
        const matrix4x4 &matrix = *(const matrix4x4*)get_scene_parameter(parameters, parameter_id);
        snapshot->light_matrix = snapshot->light_matrix * matrix;
    }

    snapshot->material_values[2].x *= material_alpha;
    snapshot->material_values[3].x *= material_alpha;

    if (has_scene_parameter(parameters,
                            fx::references::parameter_id_material_texture_matrix.read())) {

        u32 parameter_id = fx::references::parameter_id_material_texture_matrix.read();
        const matrix4x4 &matrix = *(const matrix4x4*)get_scene_parameter(parameters, parameter_id);
        snapshot->texture_matrix = snapshot->texture_matrix * matrix;
    }
}

void ngl::shaders::generated_material::prepare_regular_vertex_context(      regular_vertex_context*          context,
                                                                      const fx::general_lighting_parameters &lighting,
                                                                      const scene_snapshot*                  snapshot,
                                                                      const fx::mesh_node_data*              node_data,
                                                                      const mesh_section*                    section,
                                                                      const matrix4x4                       &local_to_world,
                                                                            bool                             receive_shadows) {

    std::memset(context, 0, sizeof(*context));

    std::memcpy(context->prefix.ambient_color_lo,
                lighting.ambient_colors,
                sizeof(context->prefix.ambient_color_lo));
    std::memcpy(context->prefix.ambient_color_hi,
                lighting.ambient_colors_with_primary,
                sizeof(context->prefix.ambient_color_hi));

    const vector4 gobo_scale(0.5f, 1.0f, 0.5f, 1.0f);

    for (u32 row = 0; row < 4; ++row) {
        for (u32 column = 0; column < 4; ++column) {
            context->prefix.local_to_gobo[row][column] =
                lighting.projector_matrix[row][column] * gobo_scale[column];
        }
    }

    if (receive_shadows) {
        context->shadow_count = shadow::build_projection_matrices(
            context->prefix.light_to_screen,
            local_to_world,
            section->sphere);
    }

    context->compressed_to_local = fx::get_compressed_to_local(node_data);
    context->prefix.compressed_to_uv =
        fx::get_compressed_to_uv() * snapshot->texture_matrix.affine();
    context->prefix.compressed_to_screen =
        context->compressed_to_local *
        local_to_world *
        ngl::references::current_scene.read()->world_to_screen;

    context->suffix.local_to_ambient     = lighting.ambient_color;
    context->suffix.fog_eye_to_local     = lighting.post_view_position;
    context->suffix.fog_normal           = lighting.post_direction;
    context->suffix.fog_position_0       = lighting.post_plane_0;
    context->suffix.fog_position_1       = lighting.post_plane_1;
    context->suffix.fogs_per_meter       = lighting.post_range;
    context->suffix.horizon_map_matrix_u = lighting.horizon_projection_u;
    context->suffix.horizon_map_matrix_v = lighting.horizon_projection_v;
}

void ngl::shaders::generated_material::configure_regular_pass_states(bool queue_class,
                                                                     u32  render_flags,
                                                                     bool use_packed_alpha) {

    d3d9::set_render_state(D3DRS_ALPHATESTENABLE, FALSE);

    if (queue_class) {
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        d3d9::set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d3d9::set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
        d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    } else
        d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);

    if ((render_flags & 0xF) == 5) {
        f32 alpha = (f32)((render_flags >> 8) & 0x1FF) * (1.0f / 256.0f);

        if (!use_packed_alpha || alpha != 1.0f || queue_class) {
            d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
            d3d9::set_render_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
            d3d9::set_render_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            d3d9::set_render_state(D3DRS_BLENDFACTOR, 0);
            d3d9::set_render_state(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            d3d9::set_render_state(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
            d3d9::set_render_state(D3DRS_SRCBLENDALPHA, D3DBLEND_ZERO);
            d3d9::set_render_state(D3DRS_DESTBLENDALPHA, D3DBLEND_DESTALPHA);
            d3d9::set_render_state(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
        }
    }

    f32 depth_bias =
        (f32)((render_flags >> 4) & 0xF) * std::bit_cast<f32>(0x35800008u);

    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, std::bit_cast<DWORD>(depth_bias));
}

void ngl::shaders::generated_material::restore_regular_pass_states() {
    d3d9::set_render_state(D3DRS_SLOPESCALEDEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_DEPTHBIAS, 0);
    d3d9::set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);
}

void ngl::shaders::generated_material::transform_light_matrix(      scene_snapshot*                  snapshot,
                                                              const fx::general_lighting_parameters &lighting) {

    snapshot->light_matrix = snapshot->light_matrix.affine() *
                             ((const matrix4x4*)(lighting.light_source + 0x100))->affine();
}

void ngl::shaders::generated_material::configure_samplers(const scene_snapshot* snapshot,
                                                          const material_data*  material) {

    for (u32 stage = 3; stage <= 10; ++stage) {
        d3d9::set_sampler_state(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MAGFILTER, snapshot->texture_filter);
        d3d9::set_sampler_state(stage, D3DSAMP_MAXANISOTROPY, 1);
    }

    for (u32 stage = 4; stage <= 10; ++stage) {
        d3d9::set_sampler_state(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        d3d9::set_sampler_state(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    }

    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    d3d9::set_sampler_state(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    d3d9::set_sampler_state(3, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
    d3d9::set_sampler_state(3, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
    d3d9::set_sampler_state(3, D3DSAMP_BORDERCOLOR, 0);

    for (u32 stage = 0; stage <= 2; ++stage) {
        d3d9::set_sampler_state(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MAXANISOTROPY, 1);
    }

    for (u32 stage = 1; stage <= 2; ++stage) {
        d3d9::set_sampler_state(stage, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
        d3d9::set_sampler_state(stage, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
        d3d9::set_sampler_state(stage, D3DSAMP_BORDERCOLOR, 0xFFFFFFFF);
    }

    d3d9::set_sampler_state(11, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(11, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(11, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    d3d9::set_sampler_state(11, D3DSAMP_MAXANISOTROPY, 1);

    DWORD horizon_address = material->horizon_texture &&
                            (material->horizon_texture->flags & texture_cube) ?
                                D3DTADDRESS_CLAMP : D3DTADDRESS_WRAP;

    d3d9::set_sampler_state(11, D3DSAMP_ADDRESSU, horizon_address);
    d3d9::set_sampler_state(11, D3DSAMP_ADDRESSV, horizon_address);
}

void ngl::shaders::generated_material::bind_textures(const fx::general_lighting_parameters &lighting,
                                                     const scene_snapshot*                  snapshot,
                                                     const material_data*                   material) {

    bind_texture( 0, lighting.horizon_texture);
    bind_texture( 1, shadow::references::device_resources.get().depth_targets[0]);
    bind_texture( 2, shadow::references::device_resources.get().depth_targets[1]);
    bind_texture( 3, lighting.projector_texture);
    bind_texture( 4, snapshot->diffuse_texture);
    bind_texture( 5, material->opacity_texture);
    bind_texture( 6, material->specularity_texture);
    bind_texture( 7, material->specular_exponent_texture);
    bind_texture( 8, material->environment_texture);
    bind_texture( 9, material->emissiveness_texture);
    bind_texture(10, material->normal_texture);
    bind_texture(11, material->horizon_texture);
}

void ngl::shaders::generated_material::upload_pixel_constants(const program_exports::pixel_pipeline_descriptor &pipeline,
                                                              const fx::general_lighting_parameters            &lighting,
                                                              const scene_snapshot*                             snapshot,
                                                              const material_data*                              material,
                                                              const matrix4x4                                  &local_to_world) {

    i32 light_count = pipeline.bindings[0];

    vector4 ambient_direction(lighting.ambient_direction.x,
                              lighting.ambient_direction.y,
                              lighting.ambient_direction.z,
                              0.0f);
    set_pixel_constant(binding(pipeline, 3), &ambient_direction);

    i32 position_register    = binding(pipeline, 4);
    i32 direction_register   = binding(pipeline, 5);
    i32 color_register       = binding(pipeline, 6);
    i32 attenuation_register = binding(pipeline, 7);

    for (i32 index = 0; index < light_count; ++index) {
        vector4 position(lighting.light_positions[index].x,
                         lighting.light_positions[index].y,
                         lighting.light_positions[index].z,
                         0.0f);
        vector4 color(lighting.light_colors[index].x,
                      lighting.light_colors[index].y,
                      lighting.light_colors[index].z,
                      0.0f);

        set_pixel_constant(position_register == -1 ? -1 : position_register + index,
                           &position);
        set_pixel_constant(direction_register == -1 ? -1 : direction_register + index,
                           &lighting.light_directions[index]);
        set_pixel_constant(color_register == -1 ? -1 : color_register + index,
                           &color);
        set_pixel_constant(attenuation_register == -1 ? -1 : attenuation_register + index,
                           &lighting.light_attenuation[index]);
    }

    set_pixel_constant(binding(pipeline, 8), &snapshot->material_vector_150);

    const vector4 &normal_value = material->normal_texture->gpu_texture.format == D3DFMT_DXT5 ?
        snapshot->material_vector_160 : references::default_normal_vector.get();
    set_pixel_constant(binding(pipeline, 9), &normal_value);

    vector4 local_x(local_to_world.x.x, local_to_world.x.y, local_to_world.x.z, 0.0f);
    vector4 local_y(local_to_world.y.x, local_to_world.y.y, local_to_world.y.z, 0.0f);
    vector4 local_z(local_to_world.z.x, local_to_world.z.y, local_to_world.z.z, 0.0f);
    set_pixel_constant(binding(pipeline, 10), &local_x);
    set_pixel_constant(binding(pipeline, 11), &local_y);
    set_pixel_constant(binding(pipeline, 12), &local_z);
    set_pixel_constant(binding(pipeline, 13), (const vector4*)&snapshot->light_matrix, 4);

    vector4 view_position(lighting.view_position.x,
                          lighting.view_position.y,
                          lighting.view_position.z,
                          0.0f);
    set_pixel_constant(binding(pipeline, 14), &view_position);

    vector4 effects_mul(lighting.reserved_1f0.y,
                        lighting.reserved_1d0.w,
                        lighting.reserved_1f0.z,
                        lighting.reserved_1d0.y);
    vector4 effects_add(lighting.reserved_200.y,
                        lighting.reserved_1e0.w,
                        lighting.reserved_200.z,
                        lighting.reserved_1e0.y);
    vector4 opacity_mad(lighting.reserved_1f0.x,
                        lighting.reserved_200.x,
                        1.0f,
                        0.0f);
    set_pixel_constant(binding(pipeline, 15), &effects_mul);
    set_pixel_constant(binding(pipeline, 16), &effects_add);
    set_pixel_constant(binding(pipeline, 17), &opacity_mad);
    set_pixel_constant(binding(pipeline, 18), &snapshot->material_vector_140);
    set_pixel_constant(binding(pipeline, 19), &references::facing_vector.get());
    set_pixel_constant(binding(pipeline, 20), &lighting.post_color);
    set_pixel_constant(binding(pipeline, 21), &lighting.fog_color);
    set_pixel_constant(binding(pipeline, 22), &lighting.fog_control);

    f32 source_scalar = *(const f32*)(lighting.light_source + 0x200);
    vector4 source_value(source_scalar, source_scalar, source_scalar, source_scalar);
    set_pixel_constant(binding(pipeline, 23), &source_value);

    vector4 horizon_color_scale(lighting.horizon_color_scale.x,
                                lighting.horizon_color_scale.y,
                                lighting.horizon_color_scale.z,
                                0.0f);
    vector4 horizon_color(lighting.horizon_color.x,
                          lighting.horizon_color.y,
                          lighting.horizon_color.z,
                          0.0f);
    vector4 horizon_axis(lighting.horizon_axis.x,
                         lighting.horizon_axis.y,
                         lighting.horizon_axis.z,
                         0.0f);
    vector4 horizon_direction_scaled(lighting.horizon_direction_scaled.x,
                                     lighting.horizon_direction_scaled.y,
                                     lighting.horizon_direction_scaled.z,
                                     0.0f);
    vector4 horizon_scalar(lighting.horizon_scalar.x,
                           lighting.horizon_scalar.x,
                           lighting.horizon_scalar.x,
                           lighting.horizon_scalar.x);
    vector4 horizon_extra(lighting.horizon_extra.x,
                          lighting.horizon_extra.y,
                          lighting.horizon_extra.z,
                          0.0f);

    set_pixel_constant(binding(pipeline, 24), &horizon_color_scale);
    set_pixel_constant(binding(pipeline, 25), &lighting.horizon_range);
    set_pixel_constant(binding(pipeline, 26), &lighting.horizon_direction);
    set_pixel_constant(binding(pipeline, 27), &horizon_color);
    set_pixel_constant(binding(pipeline, 28), &horizon_axis);
    set_pixel_constant(binding(pipeline, 29), &horizon_direction_scaled);
    set_pixel_constant(binding(pipeline, 30), &lighting.horizon_bias);
    set_pixel_constant(binding(pipeline, 31), &horizon_scalar);
    set_pixel_constant(binding(pipeline, 32), &horizon_extra);
    set_pixel_constant(binding(pipeline, 33), &shadow::references::shadow_distances.get());

    i32 ambient_register = binding(pipeline, 34);

    for (u32 index = 0; index < 9; ++index) {
        set_pixel_constant(ambient_register == -1 ? -1 : ambient_register + index,
                           &lighting.ambient_defaults[index]);
    }
}

void ngl::shaders::generated_material::draw_pixel_pipeline(
    const program_exports::pixel_pipeline_descriptor &pipeline,
    const fx::general_lighting_parameters            &lighting,
    const scene_snapshot*                             snapshot,
    const material_data*                              material,
    const matrix4x4                                  &local_to_world,
          mesh_section*                               section) {

    d3d9::set_pixel_program(*pipeline.pixel_program_output);
    configure_samplers(snapshot, material);
    bind_textures(lighting, snapshot, material);
    upload_pixel_constants(pipeline, lighting, snapshot, material, local_to_world);
    d3d9::draw_mesh_section(section);
}
