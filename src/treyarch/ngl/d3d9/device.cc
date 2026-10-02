#include "treyarch/game/game.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/frame_lock.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "banana/logging.hh"

using namespace treyarch;

// sub_72BD80
void ngl::d3d9::poison_bindings() {
    u32* cached_state = (u32*)&references::bindings.get();

    for (u32 index = 0; index < 36; ++index)
        cached_state[index] = 0xDEADBEEF;

    // retail writes this after poisoning the cache
    references::bindings.get().validation = 0x4B3C2D1E;
}

// sub_9DCAB0
void ngl::d3d9::reset_bindings() {
    IDirect3DDevice9* device = references::device.get();
    binding_cache& bindings = references::bindings.get();

    bindings.stream_sources[0] = nullptr;
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

// sub_72C260
void ngl::d3d9::set_vertex_buffer(      IDirect3DVertexBuffer9** buffer,
                                  const vertex_definition*       definition,
                                        u32                      offset,
                                        u32                      stream) {

    binding_cache &bindings = references::bindings.get();

    IDirect3DVertexDeclaration9* declaration = definition->declaration;

    // the cache stores where the buffer pointer lives, shifted by the offset and the stream
    u32 key = (stream << 16) + (u32)buffer + offset;

    if ((u32)bindings.stream_sources[stream] == key && bindings.vertex_declaration == declaration)
        return;

    IDirect3DDevice9* device = references::device.get();

    if (bindings.vertex_declaration != declaration)
        device->SetVertexDeclaration(declaration);

    device->SetStreamSource(stream, *buffer, offset, definition->vertex_size);

    bindings.stream_sources[stream] = (IDirect3DVertexBuffer9*)key;
    bindings.vertex_declaration     = declaration;
}

// sub_9DCC80
void ngl::d3d9::wait_for_rendering() {
    reset_bindings();

    ngl::references::frame_epoch.get() += 2;
}

// sub_9DCB50
void ngl::d3d9::reset_device() {
    ngl::apply_frame_lock(ngl::references::current_frame_lock.read());

    release_framebuffers();

    game::release_device_resources();

    references::presentation.get().PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    HRESULT result = references::device.get()->Reset(&references::presentation.get());

    initialize_framebuffers();

    game::restore_device_resources();

    banana::log.ngl("device reset -- 0x{:08X}", (u32)result);
}
