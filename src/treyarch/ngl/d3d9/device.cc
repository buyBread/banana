#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

void ngl::d3d9::reset_bindings() {
    IDirect3DDevice9* device = references::device.get();

    references::stream_source.write(nullptr);
    references::stream_source_stride.write(0);
    device->SetStreamSource(0, nullptr, 0, 0);

    references::indices.write(nullptr);
    device->SetIndices(nullptr);

    references::vertex_shader.write(nullptr);
    device->SetVertexShader(nullptr);

    references::pixel_shader.write(nullptr);
    device->SetPixelShader(nullptr);

    for (u32 index = 0; index < 16; ++index) {
        device->SetTexture(index, nullptr);

        references::textures.get().values[index] = nullptr;
    }
}

void ngl::d3d9::wait_for_rendering() {
    reset_bindings();

    ngl::references::frame_epoch.get() += 2;
}
