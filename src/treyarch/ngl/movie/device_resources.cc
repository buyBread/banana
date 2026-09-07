#include "treyarch/ngl/movie/device_resources.hh"
#include "treyarch/ngl/texture/runtime.hh"

using namespace treyarch;

void ngl::movie::release_device_resources() {
    device_resource_state &state = references::device_resources.get();

    for (u32 index = 0; index < 4; ++index)
        ngl::release_runtime_texture_device_resources(state.upload_targets[index]);
}

void ngl::movie::restore_device_resources() {
    device_resource_state &state = references::device_resources.get();

    for (u32 index = 0; index < 4; ++index)
        ngl::restore_runtime_texture_device_resources(state.upload_targets[index]);
}
