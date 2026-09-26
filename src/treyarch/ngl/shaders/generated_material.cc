#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/lighting_parameters.hh"
#include "treyarch/ngl/shaders/generated_material.hh"
#include "treyarch/ngl/shaders/program_exports.hh"
#include "treyarch/ngl/shadow/device_resources.hh"

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

void ngl::shaders::generated_material::transform_light_matrix(      scene_snapshot*                  snapshot,
                                                              const fx::general_lighting_parameters &lighting) {

    snapshot->light_matrix = snapshot->light_matrix.affine() * ((const matrix4x4*)(lighting.light_source + 0x100))->affine();
}

void ngl::shaders::generated_material::configure_samplers(const scene_snapshot* snapshot,
                                                          const material_data*  material) {

    DWORD material_filter = snapshot->texture_filter == 3 ?
        D3DTEXF_LINEAR : snapshot->texture_filter;

    for (u32 stage = 3; stage <= 10; ++stage) {
        d3d9::set_sampler_state(stage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        d3d9::set_sampler_state(stage, D3DSAMP_MAGFILTER, material_filter);
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
