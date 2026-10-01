#include <cstring>
#include <new>

#include "treyarch/game/post_process/post_process.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/framebuffer.hh"
#include "treyarch/ngl/d3d9/work_buffers.hh"
#include "treyarch/ngl/texture/runtime.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_765070
void post_process::create_device_resources() {
    device_resource_state &state = references::device_resources.get();

    if (state.initialized)
        return;

    IDirect3DDevice9* device = ngl::d3d9::references::device.get();

    state.initialized = 1;

    ngl::vertex_definition definition { 20, &references::half_texel_vertex_elements.get(), nullptr };
    device->CreateVertexDeclaration(definition.elements, &definition.declaration);
    state.half_texel_vertex_definition = definition;

    definition = { 16, &references::quad_vertex_elements.get(), nullptr };
    device->CreateVertexDeclaration(definition.elements, &definition.declaration);
    state.quad_vertex_definition = definition;

    ngl::texture* back_buffer = ngl::d3d9::get_back_buffer();

    i32 width  = (i32)back_buffer->gpu_texture.width;
    i32 height = (i32)back_buffer->gpu_texture.height;

    state.inverse_width  = (f32)(1.0 / (f64)(f32)width);
    state.inverse_height = (f32)(1.0 / (f64)(f32)height);

    const u32 target_flags = ngl::runtime_texture_render_target | 0x10;

    state.render_targets[0]   = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, width,      height,      1, 1);
    state.quarter_target      = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, width >> 2, height >> 2, 1, 1);
    state.eighth_target       = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, width >> 3, height >> 3, 1, 1);
    state.sixty_fourth_target = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, width >> 6, height >> 6, 1, 1);

    // position, texture coordinate
    const f32 quad[16] { -1.0f,  1.0f, 0.0f, 0.0f,
                         -1.0f, -1.0f, 0.0f, 1.0f,
                          1.0f,  1.0f, 1.0f, 0.0f,
                          1.0f, -1.0f, 1.0f, 1.0f };

    const f32 inset_quad[16] { -1.0f,  1.0f, 0.005f, 0.005f,
                               -1.0f, -1.0f, 0.005f, 0.995f,
                                1.0f,  1.0f, 0.995f, 0.005f,
                                1.0f, -1.0f, 0.995f, 0.995f };

    void* vertices;

    device->CreateVertexBuffer(4 * state.quad_vertex_definition.vertex_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &state.quad_vertex_buffer_0,
                               nullptr);
    state.quad_vertex_buffer_0->Lock(0, sizeof(quad), &vertices, 0);
    std::memcpy(vertices, quad, sizeof(quad));
    state.quad_vertex_buffer_0->Unlock();

    device->CreateVertexBuffer(4 * state.quad_vertex_definition.vertex_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &state.quad_vertex_buffer_1,
                               nullptr);
    state.quad_vertex_buffer_1->Lock(0, sizeof(inset_quad), &vertices, 0);
    std::memcpy(vertices, inset_quad, sizeof(inset_quad));
    state.quad_vertex_buffer_1->Unlock();

    /*
        texel-offset vectors; nothing in PC reads them back.
        every product is formed in double and narrowed once, and the first lane of unk_010300e0
        scales the inverse height where its neighbours use the inverse width.
    */
    {
        const f32 u = state.inverse_width;
        const f32 v = state.inverse_height;

        const f32 u_negative   = (f32)((f64)u * -1.0);
        const f32 v_negative   = (f32)((f64)v * -1.0);
        const f32 u_3          = (f32)((f64)u *  3.0);
        const f32 v_3          = (f32)((f64)v *  3.0);
        const f32 u_negative_3 = (f32)((f64)u * -3.0);
        const f32 v_negative_3 = (f32)((f64)v * -3.0);
        const f32 u_4          = (f32)((f64)u *  4.0);
        const f32 u_negative_4 = (f32)((f64)u * -4.0);
        const f32 u_6          = (f32)((f64)u *  6.0);
        const f32 u_negative_6 = (f32)((f64)u * -6.0);
        const f32 v_negative_6 = (f32)((f64)v * -6.0);

        const f32 u_8            = (f32)((f64)u * 8.0);
        const f32 v_8            = (f32)((f64)v * 8.0);
        const f32 u_negative_8   = (f32)(((f64)u * -1.0) * 8.0);
        const f32 v_negative_8   = (f32)(((f64)v * -1.0) * 8.0);
        const f32 u_24           = (f32)(((f64)u *  3.0) * 8.0);
        const f32 v_24           = (f32)(((f64)v *  3.0) * 8.0);
        const f32 u_negative_24  = (f32)(((f64)u * -3.0) * 8.0);
        const f32 v_negative_24  = (f32)(((f64)v * -3.0) * 8.0);

        references::unk_010311d0.write(vector4((f32)(((f64)u * (f64)-1.2f) * 4.0), 0.0f,
                                               (f32)(((f64)u * (f64) 1.2f) * 4.0), 0.0f));
        references::unk_01030b80.write(vector4(0.0f, (f32)(((f64)v * (f64)-1.2f) * 4.0),
                                               0.0f, (f32)(((f64)v * (f64) 1.2f) * 4.0)));

        references::unk_01030100.write(vector4(u_negative,   v_negative,   u_negative,   v));
        references::unk_01030ec0.write(vector4(u,            v_negative,   u,            v));
        references::unk_01030a20.write(vector4(u_4,          v_negative,   u_4,          v));
        references::unk_01031e40.write(vector4(u_negative_4, v_negative,   u_negative_4, v));
        references::unk_01030930.write(vector4(u_6,          v_negative,   u_6,          v));
        references::unk_010300e0.write(vector4(v_negative_6, v_negative,   u_negative_6, v));
        references::unk_01030eb0.write(vector4(u_negative_3, v_negative_3, u_negative,   v_negative_3));
        references::unk_01030a60.write(vector4(u,            v_negative_3, u_3,          v_negative_3));
        references::unk_01031f70.write(vector4(u_negative_3, v_negative,   u_negative,   v_negative));
        references::unk_01031ac0.write(vector4(u,            v_negative,   u_3,          v_negative));
        references::unk_01031e30.write(vector4(u_negative_3, v,            u_negative,   v));
        references::unk_01030b00.write(vector4(u,            v,            u_3,          v));
        references::unk_01031f80.write(vector4(u_negative_3, v_3,          u_negative,   v_3));
        references::unk_01031de0.write(vector4(u,            v_3,          u_3,          v_3));

        references::unk_01030080.write(vector4(u_negative_24, v_negative_24, u_negative_8, v_negative_24));
        references::unk_01032010.write(vector4(u_8,           v_negative_24, u_24,         v_negative_24));
        references::unk_01031dc0.write(vector4(u_negative_24, v_negative_8,  u_negative_8, v_negative_8));
        references::unk_01031d70.write(vector4(u_8,           v_negative_8,  u_24,         v_negative_8));
        references::unk_01031d50.write(vector4(u_negative_24, v_8,           u_negative_8, v_8));
        references::unk_01030b20.write(vector4(u_8,           v_8,           u_24,         v_8));
        references::unk_01032000.write(vector4(u_negative_24, v_24,          u_negative_8, v_24));
        references::unk_01031d40.write(vector4(u_8,           v_24,          u_24,         v_24));
    }

    references::bloom_enabled.write(1);

    i32 half_width  = width  >> 1;
    i32 half_height = height >> 1;

    state.render_targets[1] = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, half_width, half_height, 1, 1);
    state.render_targets[2] = ngl::create_runtime_texture(target_flags, D3DFMT_A8R8G8B8, half_width, half_height, 1, 1);

    f32 half_texel_u = (f32)(0.5 / (f64)(f32)half_width);
    f32 half_texel_v = (f32)(0.5 / (f64)(f32)half_height);

    f32 u_0 = (f32)((f64)half_texel_u + 0.0);
    f32 u_1 = (f32)((f64)half_texel_u + 1.0);
    f32 v_0 = (f32)((f64)half_texel_v + 0.0);
    f32 v_1 = (f32)((f64)half_texel_v + 1.0);

    // two triangles, position then texture coordinate, sampling texel centres of the half-size targets
    const f32 half_texel_quad[30] { -1.0f,  1.0f, 0.0f, u_0, v_0,
                                    -1.0f, -1.0f, 0.0f, u_0, v_1,
                                     1.0f,  1.0f, 0.0f, u_1, v_0,
                                     1.0f,  1.0f, 0.0f, u_1, v_0,
                                    -1.0f, -1.0f, 0.0f, u_0, v_1,
                                     1.0f, -1.0f, 0.0f, u_1, v_1 };

    device->CreateVertexBuffer(6 * state.half_texel_vertex_definition.vertex_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &state.half_texel_vertex_buffer_0,
                               nullptr);
    state.half_texel_vertex_buffer_0->Lock(0, sizeof(half_texel_quad), &vertices, 0);
    std::memcpy(vertices, half_texel_quad, sizeof(half_texel_quad));
    state.half_texel_vertex_buffer_0->Unlock();

    device->CreateVertexBuffer(6 * state.half_texel_vertex_definition.vertex_size,
                               0,
                               0,
                               D3DPOOL_MANAGED,
                               &state.half_texel_vertex_buffer_1,
                               nullptr);
    state.half_texel_vertex_buffer_1->Lock(0, sizeof(half_texel_quad), &vertices, 0);
    std::memcpy(vertices, half_texel_quad, sizeof(half_texel_quad));
    state.half_texel_vertex_buffer_1->Unlock();

    f32 tap_scale_u = (f32)(1.0 / (f64)(f32)half_width);
    f32 tap_scale_v = (f32)(1.0 / (f64)(f32)half_height);

    // nothing sets the guard, so every device reset scales the taps again
    if (!references::pause_menu_blur_taps_scaled.read()) {
        blur_taps &horizontal = references::pause_menu_blur_horizontal_taps.get();
        blur_taps &vertical   = references::pause_menu_blur_vertical_taps.get();

        for (u32 tap = 0; tap < 13; ++tap) {
            horizontal.taps[tap].x = (f32)((f64)horizontal.taps[tap].x * (f64)tap_scale_u);
            vertical.taps[tap].y   = (f32)((f64)vertical.taps[tap].y   * (f64)tap_scale_v);
        }
    }

    if (!state.system) {
        void* allocation = memory::heap::allocate(sizeof(post_process::system));

        state.system = allocation ? new (allocation) post_process::system() : nullptr;
        state.system->create_render_targets((u8*)ngl::d3d9::references::platform_work_buffer.read());

        apply_default_parameters();
    }
}

// sub_7660F0
void post_process::release_device_resources() {
    device_resource_state &state = references::device_resources.get();

    if (!state.initialized)
        return;

    state.initialized = 0;

    state.half_texel_vertex_buffer_0->Release();
    state.half_texel_vertex_buffer_1->Release();

    for (u32 index = 0; index < 3; ++index) {
        if (state.render_targets[index])
            ngl::release_texture(state.render_targets[index]);
    }

    if (state.quarter_target)
        ngl::release_texture(state.quarter_target);

    state.quarter_target = nullptr;

    if (state.eighth_target)
        ngl::release_texture(state.eighth_target);

    if (state.sixty_fourth_target)
        ngl::release_texture(state.sixty_fourth_target);

    state.eighth_target       = nullptr;
    state.sixty_fourth_target = nullptr;

    state.quad_vertex_buffer_0->Release();
    state.quad_vertex_buffer_1->Release();

    if (state.system) {
        post_process::system* released = state.system;

        released->release();
        memory::heap::free(released);
        state.system = nullptr;
    }
}
