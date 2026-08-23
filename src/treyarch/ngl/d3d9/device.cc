#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/texture/texture.hh"

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
    bindings.vertex_declaration = nullptr;
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

void ngl::d3d9::set_texture(u32 stage, IDirect3DBaseTexture9* value) {
    binding_cache &bindings = references::bindings.get();

    if (bindings.textures[stage] == value)
        return;

    bindings.textures[stage] = value;
    references::device.get()->SetTexture(stage, value);
}

void ngl::d3d9::set_vertex_program(IDirect3DVertexShader9* value) {
    binding_cache &bindings = references::bindings.get();

    if (bindings.vertex_shader == value)
        return;

    bindings.vertex_shader = value;
    references::device.get()->SetVertexShader(value);
}

void ngl::d3d9::set_pixel_program(IDirect3DPixelShader9* value) {
    binding_cache &bindings = references::bindings.get();

    if (bindings.pixel_shader == value)
        return;

    bindings.pixel_shader = value;
    references::device.get()->SetPixelShader(value);
}

void ngl::d3d9::set_vertex_definition(const vertex_definition* value) {
    IDirect3DVertexDeclaration9* declaration = value ?
        value->declaration : nullptr;

    binding_cache &bindings = references::bindings.get();

    if (bindings.vertex_declaration == declaration)
        return;

    bindings.vertex_declaration = declaration;
    references::device.get()->SetVertexDeclaration(declaration);
}

void ngl::d3d9::wait_for_rendering() {
    reset_bindings();

    ngl::references::frame_epoch.get() += 2;
}

void ngl::d3d9::reset_device() {
    framebuffer_state &framebuffers = references::framebuffers.get();

    ngl::release_texture(framebuffers.back_buffer);
    ngl::release_texture(framebuffers.secondary_hdr_buffer);
    ngl::release_texture(framebuffers.secondary_ldr_buffer);
    ngl::release_texture(framebuffers.front_buffer);
    ngl::release_texture(framebuffers.linear_depth_buffer);

    references::presentation.get().Windowed = FALSE;
    references::device.get()->Reset(&references::presentation.get());

    initialize_framebuffers();
    poison_bindings();
    reset_bindings();
}
