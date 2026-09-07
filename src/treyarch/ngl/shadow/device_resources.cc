#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/ngl/shadow/device_resources.hh"

using namespace treyarch;

void ngl::shadow::release_device_resources() {
    if (!references::initialized.read())
        return;

    device_resource_state &state = references::device_resources.get();

    for (u32 index = 0; index < 2; ++index) {
        ngl::release_runtime_texture_device_resources(state.depth_targets[index]);
        ngl::release_runtime_texture_device_resources(state.color_targets[index]);
    }
}

void ngl::shadow::restore_device_resources() {
    if (!references::initialized.read())
        return;

    device_resource_state &state = references::device_resources.get();

    for (u32 index = 0; index < 2; ++index) {
        ngl::restore_runtime_texture_device_resources(state.depth_targets[index]);
        ngl::restore_runtime_texture_device_resources(state.color_targets[index]);
    }
}
