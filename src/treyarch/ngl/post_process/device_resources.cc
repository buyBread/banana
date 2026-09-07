#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/ngl/post_process/device_resources.hh"

using namespace treyarch;

void ngl::post_process::surface_texture::release_device_resources() {
    saved_color_target = nullptr;
    saved_depth_target = nullptr;

    if (render_target && owns_render_target) {
        render_target->Release();
        render_target = nullptr;
    }
}

void ngl::post_process::surface_texture::restore_device_resources() {
    if (!owns_render_target || render_target)
        return;

    ngl::d3d9::create_surface_resource(&render_target,
                                       texture.width,
                                       texture.height,
                                       texture.format);
}

void ngl::post_process::mipmap_texture_array::release_device_resources() {
    for (u32 index = 0; index < count; ++index)
        textures[index].release_device_resources();
}

void ngl::post_process::mipmap_texture_array::restore_device_resources() {
    for (u32 index = 0; index < count; ++index)
        textures[index].restore_device_resources();
}

void ngl::post_process::system::release_device_resources() {
    if (hdr_targets)
        hdr_targets->release_device_resources();

    if (ldr_targets)
        ldr_targets->release_device_resources();

    if (single_target_0)
        single_target_0->release_device_resources();

    if (single_target_1)
        single_target_1->release_device_resources();

    if (secondary_ldr_targets_0)
        secondary_ldr_targets_0->release_device_resources();

    if (secondary_ldr_targets_1)
        secondary_ldr_targets_1->release_device_resources();

    if (single_target_2)
        single_target_2->release_device_resources();
}

void ngl::post_process::system::restore_device_resources() {
    if (hdr_targets)
        hdr_targets->restore_device_resources();

    if (ldr_targets)
        ldr_targets->restore_device_resources();

    if (single_target_0)
        single_target_0->restore_device_resources();

    if (single_target_1)
        single_target_1->restore_device_resources();

    if (secondary_ldr_targets_0)
        secondary_ldr_targets_0->restore_device_resources();

    if (secondary_ldr_targets_1)
        secondary_ldr_targets_1->restore_device_resources();

    if (single_target_2)
        single_target_2->restore_device_resources();
}

void ngl::post_process::release_device_resources() {
    device_resource_state &state = references::device_resources.get();

    if (!state.initialized)
        return;

    for (u32 index = 0; index < 3; ++index)
        ngl::release_runtime_texture_device_resources(state.render_targets[index]);

    ngl::release_runtime_texture_device_resources(state.quarter_target);
    ngl::release_runtime_texture_device_resources(state.eighth_target);
    ngl::release_runtime_texture_device_resources(state.sixty_fourth_target);

    if (state.system)
        state.system->release_device_resources();
}

void ngl::post_process::restore_device_resources() {
    device_resource_state &state = references::device_resources.get();

    if (!state.initialized)
        return;

    for (u32 index = 0; index < 3; ++index)
        ngl::restore_runtime_texture_device_resources(state.render_targets[index]);

    ngl::restore_runtime_texture_device_resources(state.quarter_target);
    ngl::restore_runtime_texture_device_resources(state.eighth_target);
    ngl::restore_runtime_texture_device_resources(state.sixty_fourth_target);

    if (state.system)
        state.system->restore_device_resources();
}
