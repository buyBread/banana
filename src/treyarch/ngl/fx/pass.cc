#include <bit>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/fx/pass.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/texture/texture.hh"

using namespace treyarch;

// sub_9EA2E0
void apply_render_states(const ngl::fx::render_states &states) {
    using namespace ngl::d3d9;

    if (states.alpha_blend_enabled) {
        set_render_state(D3DRS_ALPHABLENDENABLE, TRUE);
        set_render_state(D3DRS_SRCBLEND, states.source_blend);
        set_render_state(D3DRS_DESTBLEND, states.destination_blend);
        set_render_state(D3DRS_BLENDFACTOR, states.texture_factor);
        set_render_state(D3DRS_BLENDOP, states.blend_operation);
    } else
        set_render_state(D3DRS_ALPHABLENDENABLE, FALSE);

    if (states.alpha_test_enabled) {
        set_render_state(D3DRS_ALPHATESTENABLE, TRUE);
        set_render_state(D3DRS_ALPHAFUNC, states.alpha_function);
        set_render_state(D3DRS_ALPHAREF, states.alpha_reference);
    } else
        set_render_state(D3DRS_ALPHATESTENABLE, FALSE);

    // mean the opposite thing in NGL idk?
    DWORD cull_mode = D3DCULL_NONE;

    if (states.cull_mode == D3DCULL_CCW)
        cull_mode = D3DCULL_CW;
    else if (states.cull_mode == D3DCULL_CW)
        cull_mode = D3DCULL_CCW;

    set_render_state(D3DRS_CULLMODE, cull_mode);
    set_render_state(D3DRS_FILLMODE, states.fill_mode);
    set_render_state(D3DRS_SHADEMODE, states.shade_mode);

    if (states.depth_test_enabled) {
        set_render_state(D3DRS_ZENABLE, TRUE);
        set_render_state(D3DRS_ZFUNC, states.depth_function);
    } else
        set_render_state(D3DRS_ZENABLE, FALSE);

    set_render_state(D3DRS_ZWRITEENABLE, states.depth_write_enabled != 0);

    if (states.stencil_enabled) {
        set_render_state(D3DRS_STENCILENABLE, TRUE);
        set_render_state(D3DRS_STENCILPASS, states.stencil_pass);
        set_render_state(D3DRS_STENCILFAIL, states.stencil_fail);
        set_render_state(D3DRS_STENCILZFAIL, states.stencil_depth_fail);
        set_render_state(D3DRS_STENCILFUNC, states.stencil_function);
        set_render_state(D3DRS_STENCILREF, states.stencil_reference);
        set_render_state(D3DRS_STENCILMASK, states.stencil_mask);
        set_render_state(D3DRS_STENCILWRITEMASK, states.stencil_write_mask);
    } else
        set_render_state(D3DRS_STENCILENABLE, FALSE);

    if (states.point_sprite_enabled) {
        set_render_state(D3DRS_POINTSPRITEENABLE, TRUE);
        set_render_state(D3DRS_POINTSIZE, std::bit_cast<DWORD>(states.point_size));
        set_render_state(D3DRS_POINTSCALEENABLE, FALSE);
        set_render_state(D3DRS_POINTSIZE_MIN, std::bit_cast<DWORD>(states.point_size_minimum));
        set_render_state(D3DRS_POINTSIZE_MAX, std::bit_cast<DWORD>(states.point_size_maximum));
    } else
        set_render_state(D3DRS_POINTSPRITEENABLE, FALSE);
}

void apply_sampler_states(u32 stage, const u32* sampler) {
    using namespace ngl::d3d9;

    set_sampler_address(stage, sampler[1], sampler[2], sampler[3]);
    set_sampler_filters(stage, sampler[5], sampler[6], sampler[4], 1);
}

void bind_texture(      ngl::fx::effect*           effect_data,
                  const ngl::fx::function_binding &binding,
                        void**                     sources) {

    ngl::texture* texture = *(ngl::texture**)((u8*)sources[binding.source_class] + binding.source_offset);

    ngl::d3d9::set_texture(binding.handle, texture->gpu_texture.resource);

    ngl::fx::parameter &parameter = effect_data->parameters[binding.parameter_index];
    apply_sampler_states(binding.handle, (const u32*)parameter.data);
}

// sub_9EA5B0
void bind_vertex_function(ngl::fx::effect*   effect_data,
                          ngl::fx::function* value,
                          void**             sources) {

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    if (!value) {
        ngl::d3d9::set_vertex_program(nullptr);

        return;
    }

    ngl::d3d9::set_vertex_program((IDirect3DVertexShader9*)value->shader);

    for (i32 index = 0; index < value->binding_count; ++index) {
        const ngl::fx::function_binding &binding = value->bindings[index];

        if (binding.destination_type == ngl::fx::binding_sampler) {
            bind_texture(effect_data, binding, sources);

            continue;
        }

        const ngl::fx::parameter &parameter = effect_data->parameters[binding.parameter_index];
        const f32* source = (const f32*)((u8*)sources[binding.source_class] + binding.source_offset);
        f32 converted[4];

        if (parameter.type == ngl::fx::parameter_bool || parameter.type == ngl::fx::parameter_int) {
            converted[0] = (f32)*(const i32*)source;
            converted[1] = 0.0f;
            converted[2] = 0.0f;
            converted[3] = 0.0f;
            source = converted;
        }

        device->SetVertexShaderConstantF(binding.handle, source, binding.register_count);
    }
}

// sub_9EA730
void bind_pixel_function(ngl::fx::effect*   effect_data,
                         ngl::fx::function* value,
                         void**             sources) {

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    if (!value) {
        ngl::d3d9::set_pixel_program(nullptr);
        return;
    }

    ngl::d3d9::set_pixel_program((IDirect3DPixelShader9*)value->shader);

    for (i32 index = 0; index < value->binding_count; ++index) {
        const ngl::fx::function_binding &binding = value->bindings[index];
        const u8* source = (const u8*)sources[binding.source_class] + binding.source_offset;

        switch (binding.destination_type) {
            case ngl::fx::binding_float:
                device->SetPixelShaderConstantF(binding.handle,
                                                 (const f32*)source,
                                                 binding.register_count);
                break;

            case ngl::fx::binding_int: {
                i32 converted[4] { *(const i32*)source, 0, 0, 0 };
                device->SetPixelShaderConstantI(binding.handle, converted, 1);
                break;
            }

            case ngl::fx::binding_bool: {
                BOOL converted = *source != 0;
                device->SetPixelShaderConstantB(binding.handle, &converted, 1);
                break;
            }

            case ngl::fx::binding_sampler:
                if (binding.handle != (u32)-1)
                    bind_texture(effect_data, binding, sources);
                break;
        }
    }
}

// sub_9EA900
void unbind_samplers(const ngl::fx::function &value) {
    for (i32 index = 0; index < value.binding_count; ++index) {
        const ngl::fx::function_binding &binding = value.bindings[index];

        if (binding.destination_type != ngl::fx::binding_sampler)
            continue;

        ngl::d3d9::set_texture(binding.handle, nullptr);
    }
}

// sub_9EA950
void ngl::fx::apply_pass(effect* effect_data, pass* value) {
    void* sources[8] {};

    sources[binding_parameters] = effect_data->parameter_data;
    sources[binding_scene] = ngl::references::current_scene.read();
    sources[binding_bones] = &ngl::fx::references::bone_constants.get();
    sources[binding_global_constants] = &ngl::fx::references::global_constants.get();
    sources[binding_fallback_texture] = references::parameter_texture_fallback_pointer.read();

    apply_render_states(value->states);

    program_set* programs = value->active_programs;

    bind_pixel_function(effect_data, &programs->pixel_programs[0], sources);
    bind_vertex_function(effect_data, &programs->vertex_programs[0], sources);
}

// sub_9EA9D0
void ngl::fx::finish_pass(effect* effect_data, pass* value) {
    (void)effect_data;

    unbind_samplers(value->active_programs->pixel_programs[0]);

    scene* current_scene = ngl::references::current_scene.read();

    ngl::d3d9::set_render_state(D3DRS_ZWRITEENABLE, current_scene->z_write_enabled);
    ngl::d3d9::set_render_state(D3DRS_COLORWRITEENABLE, current_scene->framebuffer_write_mask);
}
