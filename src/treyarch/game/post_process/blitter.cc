#include "treyarch/game/post_process/blitter.hh"
#include "treyarch/game/shader_resource_manager.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"

using namespace treyarch;

// sub_73C090
blitter::blitter() {
    vtable          = &references::blitter_vtable.get();
    reference_count = 1;

    for (u32 tap = 0; tap < 16; ++tap) {
        tap_scale_u[tap]  = 1.0f;
        tap_scale_v[tap]  = 1.0f;
        tap_offset_u[tap] = 0.0f;
        tap_offset_v[tap] = 0.0f;
    }
}

// sub_75E8A0
bool blitter::initialize() {
    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    vertex_elements[0] = { 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 };
    vertex_elements[1] = { 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 };
    vertex_elements[2] = D3DDECL_END();

    vertex_format.elements    = vertex_elements;
    vertex_format.vertex_size = 16;
    vertex_format.declaration = nullptr;

    device->CreateVertexDeclaration(vertex_elements, &vertex_format.declaration);

    shader_resource_manager* programs = references::shader_resource_manager.read();

    vertex_program              = programs->find_vertex_program("blitter_vs");
    texture_program             = programs->find_pixel_program ("blitter_texture_ps");
    solid_color_program         = programs->find_pixel_program ("blitter_solid_color_ps");
    depth_texture_program       = programs->find_pixel_program ("blitter_depth_texture_ps");
    color_depth_texture_program = programs->find_pixel_program ("blitter_color_depth_texture_ps");

    device->CreateVertexBuffer(3 * vertex_format.vertex_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &vertex_buffer,
                               nullptr);

    f32* vertices = nullptr;
    vertex_buffer->Lock(0, 0, (void**)&vertices, 0);

    if (!vertices)
        return false;

    // one oversized triangle covering the viewport: position, texture coordinate
    vertices[ 0] = 0.0f; vertices[ 1] = 0.0f; vertices[ 2] = 0.0f; vertices[ 3] = 0.0f;
    vertices[ 4] = 2.0f; vertices[ 5] = 0.0f; vertices[ 6] = 2.0f; vertices[ 7] = 0.0f;
    vertices[ 8] = 0.0f; vertices[ 9] = 2.0f; vertices[10] = 0.0f; vertices[11] = 2.0f;

    vertex_buffer->Unlock();

    return true;
}

// inlined into sub_75EB00
void blitter::release() {
    using deleting_destructor = void*(__thiscall*)(blitter*, u8);

    if ((i32)reference_count > 0 && !--reference_count)
        ((deleting_destructor*)vtable)[0](this, 1);
}

// sub_72F210
void blitter::draw_fullscreen() {
    ngl::d3d9::binding_cache &bindings = ngl::d3d9::references::bindings.get();
    
    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);

    ngl::d3d9::set_vertex_program(*vertex_program);

    u32 stream_key = (u32)&vertex_buffer;

    if ((u32)bindings.stream_source != stream_key ||
        bindings.vertex_declaration != vertex_format.declaration) {

        if (bindings.vertex_declaration != vertex_format.declaration) {
            bindings.vertex_declaration = vertex_format.declaration;
            device->SetVertexDeclaration(vertex_format.declaration);
        }

        device->SetStreamSource(0, vertex_buffer, 0, vertex_format.vertex_size);
        bindings.stream_source = (IDirect3DVertexBuffer9*)stream_key;
    }

    f32 constants[19][4] {};

    for (u32 tap = 0; tap < 16; ++tap) {
        constants[tap][0] = tap_scale_u[tap];
        constants[tap][1] = tap_scale_v[tap];
        constants[tap][2] = tap_offset_u[tap];
        constants[tap][3] = tap_offset_v[tap];
    }

    constants[16][0] =  2.0f;
    constants[16][1] =  2.0f;
    constants[16][2] = -1.0f - 1.0f / (f32)viewport.Width;
    constants[16][3] = -1.0f + 1.0f / (f32)viewport.Height;
    constants[17][0] =  1.0f;
    constants[17][1] = -1.0f;
    constants[17][2] =  0.0f;
    constants[17][3] =  1.0f;
    constants[18][2] =  1.0f;
    constants[18][3] =  1.0f;

    device->SetVertexShaderConstantF(0, &constants[0][0], 19);

    RECT scissor { (LONG)viewport.X,
                   (LONG)viewport.Y,
                   (LONG)(viewport.X + viewport.Width),
                   (LONG)(viewport.Y + viewport.Height) };

    device->SetViewport(&viewport);
    ngl::d3d9::set_render_state(D3DRS_SCISSORTESTENABLE, TRUE);
    device->SetScissorRect(&scissor);
    ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
    device->SetViewport(&viewport);
    device->SetScissorRect(&scissor);
}
