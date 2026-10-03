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

// sub_72ED70
void blitter::draw(f32 x,
                   f32 y,
                   f32 width,
                   f32 height,
                   f32 u,
                   f32 v,
                   f32 u_size,
                   f32 v_size,
                   f32 z) {

    // nothing to draw when the rectangle sits entirely off screen
    f64 right = (f64)x + (f64)width;

    if (0.0 > right || x > 1.0f)
        return;

    f64 bottom = (f64)y + (f64)height;

    if (0.0 > bottom || y > 1.0f)
        return;

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);

    i32 viewport_x      = (i32)viewport.X;
    i32 viewport_y      = (i32)viewport.Y;
    i32 viewport_width  = (i32)viewport.Width;
    i32 viewport_height = (i32)viewport.Height;

    f32 constants[19][4];

    for (u32 tap = 0; tap < 16; ++tap) {
        constants[tap][0] = tap_scale_u[tap];
        constants[tap][1] = tap_scale_v[tap];
        constants[tap][2] = tap_offset_u[tap];
        constants[tap][3] = tap_offset_v[tap];
    }

    f32 width_scale  = (f32)viewport_width;
    f32 height_scale = (f32)viewport_height;

    f32 left_edge = (f32)((f64)x * 2.0 - 1.0);
    f32 top_edge  = (f32)((f64)y * 2.0 - 1.0);

    // position scale and offset, nudged by half a pixel's worth of clip space
    constants[16][0] = (f32)((f64)width  * 2.0);
    constants[16][1] = (f32)((f64)height * 2.0);
    constants[16][2] = (f32)((f64)left_edge - 1.0 / (f64)width_scale);
    constants[16][3] = (f32)(1.0 / (f64)height_scale + (f64)top_edge);

    // texture coordinate scale and offset, v flipped
    constants[17][0] = u_size;
    constants[17][1] = -v_size;
    constants[17][2] = u;
    constants[17][3] = (f32)(1.0 - (f64)v);

    constants[18][0] = 0.0f;
    constants[18][1] = 0.0f;
    constants[18][2] = z;
    constants[18][3] = 1.0f;

    ngl::d3d9::set_vertex_program(*vertex_program);
    ngl::d3d9::set_vertex_buffer(&vertex_buffer, &vertex_format, 0, 0);

    device->SetVertexShaderConstantF(0, &constants[0][0], 19);

    i32 clipped_left   = viewport_x + (i32)((f64)width_scale  * (f64)x);
    i32 clipped_right  = viewport_x + (i32)((f64)width_scale  * right);
    i32 clipped_top    = viewport_y + (i32)((f64)height_scale * (f64)y);
    i32 clipped_bottom = viewport_y + (i32)((f64)height_scale * bottom);

    if (clipped_left < viewport_x)
        clipped_left = viewport_x;

    i32 viewport_right = viewport_x + viewport_width;

    if (clipped_right > viewport_right)
        clipped_right = viewport_right;

    if (clipped_top < viewport_y)
        clipped_top = viewport_y;

    i32 viewport_bottom = viewport_y + viewport_height;

    if (clipped_bottom > viewport_bottom)
        clipped_bottom = viewport_bottom;

    D3DVIEWPORT9 clipped { (DWORD)clipped_left,
                           (DWORD)clipped_top,
                           (DWORD)(clipped_right - clipped_left),
                           (DWORD)(clipped_bottom - clipped_top),
                           0.0f,
                           1.0f };

    device->SetViewport(&clipped);
    ngl::d3d9::set_scissor(clipped_left, clipped_top, clipped_right, clipped_bottom);
    ngl::d3d9::set_render_state(D3DRS_CULLMODE, D3DCULL_CW);
    device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    // the depth range comes back as 0..1, not whatever the saved viewport had
    D3DVIEWPORT9 restored { viewport.X,
                            viewport.Y,
                            viewport.Width,
                            viewport.Height,
                            0.0f,
                            1.0f };

    device->SetViewport(&restored);
    ngl::d3d9::set_scissor(viewport_x, viewport_y, viewport_right, viewport_bottom);
}

// sub_72F210
void blitter::draw_fullscreen() {
    draw(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
}

// sub_72F250
void blitter::draw_texture(f32                     x,
                           f32                     y,
                           f32                     width,
                           f32                     height,
                           f32                     u,
                           f32                     v,
                           f32                     u_size,
                           f32                     v_size,
                           f32                     z,
                           IDirect3DBaseTexture9** texture) {

    ngl::d3d9::set_pixel_program(*texture_program);
    ngl::d3d9::set_texture(0, *texture);

    draw(x, y, width, height, u, v, u_size, v_size, z);

    // the unbind skips the cache check
    ngl::d3d9::references::device.get()->SetTexture(0, nullptr);
    ngl::d3d9::references::bindings.get().textures[0] = nullptr;
}

// sub_72F320
void blitter::draw_texture_fullscreen(IDirect3DBaseTexture9** texture) {
    draw_texture(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, texture);
}
