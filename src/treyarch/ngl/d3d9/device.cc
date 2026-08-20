#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

void ngl::d3d9::poison_bindings() {
    u32* cached_state = (u32*)&references::bindings.get();

    for (u32 index = 0; index < 36; ++index)
        cached_state[index] = 0xDEADBEEF;

    references::bindings.get().validation = 0x4B3C2D1E;
}

void ngl::d3d9::reset_bindings() {
    IDirect3DDevice9* device = references::device.get();
    binding_cache& bindings = references::bindings.get();

    bindings.stream_source = nullptr;
    bindings.stream_source_stride = 0;
    device->SetStreamSource(0, nullptr, 0, 0);

    bindings.indices = nullptr;
    device->SetIndices(nullptr);

    bindings.vertex_shader = nullptr;
    device->SetVertexShader(nullptr);

    bindings.pixel_shader = nullptr;
    device->SetPixelShader(nullptr);

    for (u32 index = 0; index < 16; ++index) {
        device->SetTexture(index, nullptr);

        bindings.textures[index] = nullptr;
    }
}

void ngl::d3d9::wait_for_rendering() {
    reset_bindings();

    ngl::references::frame_epoch.get() += 2;
}
