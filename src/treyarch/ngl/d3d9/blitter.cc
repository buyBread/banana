#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/state_cache.hh"
#include "treyarch/ngl/d3d9/blitter.hh"

using namespace treyarch;

void ngl::d3d9::blitter::draw_fullscreen() {
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
    const f32* source = (const f32*)transforms;

    for (u32 row = 0; row < 16; ++row) {
        for (u32 column = 0; column < 4; ++column)
            constants[row][column] = source[column * 16 + row];
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
