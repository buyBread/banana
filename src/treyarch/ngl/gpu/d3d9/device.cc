#include <cstring>
#include <d3d9.h>
#include <windows.h>

#include "banana/logging.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/frame_lock.hh"
#include "treyarch/ngl/gpu/d3d9/device.hh"
#include "treyarch/ngl/gpu/d3d9/display.hh"
#include "treyarch/ngl/gpu/d3d9/framebuffer.hh"
#include "treyarch/ngl/gpu/d3d9/geometry_stream.hh"
#include "treyarch/ngl/gpu/d3d9/internal_programs.hh"
#include "treyarch/ngl/gpu/d3d9/scene.hh"
#include "treyarch/ngl/gpu/d3d9/state_cache.hh"
#include "treyarch/ngl/gpu/d3d9/vertex_formats.hh"
#include "treyarch/ngl/gpu/d3d9/work_buffers.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "game/container/legacy_list.hh"
#include "game/game.hh"
#include "game/timing/hires_clock.hh"

namespace treyarch { namespace ngl { namespace d3d9 { namespace references {
    util::memory_reference<container::legacy_list<void*>*> frame_owned_objects { 0x00F532A8 };
}}}} // treyarch::ngl::d3d9::references

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
    IDirect3DDevice9* device = references::dev.get();
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
    references::dev.get()->SetTexture(stage, value);
}

void ngl::d3d9::set_vertex_program(IDirect3DVertexShader9* value) {
    binding_cache &bindings = references::bindings.get();

    if (bindings.vertex_shader == value)
        return;

    bindings.vertex_shader = value;
    references::dev.get()->SetVertexShader(value);
}

void ngl::d3d9::set_pixel_program(IDirect3DPixelShader9* value) {
    binding_cache &bindings = references::bindings.get();

    if (bindings.pixel_shader == value)
        return;

    bindings.pixel_shader = value;
    references::dev.get()->SetPixelShader(value);
}

void ngl::d3d9::set_vertex_definition(const vertex_definition* value) {
    IDirect3DVertexDeclaration9* declaration = value ?
        value->declaration : nullptr;

    binding_cache &bindings = references::bindings.get();

    if (bindings.vertex_declaration == declaration)
        return;

    bindings.vertex_declaration = declaration;
    references::dev.get()->SetVertexDeclaration(declaration);
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

    IDirect3DDevice9* device = references::dev.get();

    if (bindings.vertex_declaration != declaration)
        device->SetVertexDeclaration(declaration);

    device->SetStreamSource(stream, *buffer, offset, definition->vertex_size);

    bindings.stream_sources[stream] = (IDirect3DVertexBuffer9*)key;
    bindings.vertex_declaration     = declaration;
}

// inlined @ sub_7AD120, sub_7CC710, sub_7CD8C0
void ngl::d3d9::set_index_buffer(IDirect3DIndexBuffer9** buffer) {
    binding_cache &bindings = references::bindings.get();

    // like the vertex streams, the cache holds where the buffer pointer lives
    if ((u32)bindings.indices == (u32)buffer)
        return;

    bindings.indices = (IDirect3DIndexBuffer9*)buffer;

    references::dev.get()->SetIndices(*buffer);
}

// sub_9DCC80
void ngl::d3d9::wait_for_rendering() {
    reset_bindings();

    ngl::references::frame.get() += 2;
}

// sub_9DCB50
void ngl::d3d9::reset_device() {
    set_frame_lock_params(ngl::references::frame_lock.read());

    release_framebuffers();

    game::release_device_resources();

    references::present_params.get().PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    HRESULT result = references::dev.get()->Reset(&references::present_params.get());

    initialize_framebuffers();

    game::restore_device_resources();

    banana::log.ngl("device reset -- 0x{:08X}", (u32)result);
}

// sub_9DCBA0
ULONG ngl::d3d9::init() {
    references::d3d.write(Direct3DCreate9(D3D_SDK_VERSION));

    select_default_display_mode();
    initialize_render_window();
    init_present_params();

    D3DDEVTYPE device_type = (D3DDEVTYPE)
        ((references::force_reference_device.read() != 0) + 1);

    HRESULT result = references::d3d.get()->CreateDevice( 0,
                                                          device_type,
                                                          ngl::references::render_window.read(),
                                                          D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                                         &references::present_params.get(),
                                                         &references::dev.get());

    const D3DPRESENT_PARAMETERS &presentation = references::present_params.get();

    banana::log.ngl("device created -- 0x{:08X} -- {} | {}x{} | {} | back buffer format: {} | refresh: {}Hz | interval: 0x{:08X}",
                    (u32)result,
                    device_type == D3DDEVTYPE_REF ? "reference" : "hardware",
                    presentation.BackBufferWidth,
                    presentation.BackBufferHeight,
                    presentation.Windowed ? "windowed" : "fullscreen",
                    (u32)presentation.BackBufferFormat,
                    presentation.FullScreen_RefreshRateInHz,
                    presentation.PresentationInterval);

    ngli_init_work_buffers();
    references::dev.get()->GetDeviceCaps(&references::caps.get());
    initialize_sampler_filters();
    initialize_internal_vertex_formats();
    initialize_internal_programs();
    
    poison_bindings();
    reset_bindings();
    
    timing::restart_frame_timer();

    return initialize_framebuffers();
}

void pump_window_messages() {
    MSG message;

    while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

// sub_9E8E00
void ngl::d3d9::flip() {
    IDirect3DDevice9* device = references::dev.get();

    device->BeginScene();

    IDirect3DSurface9* device_back_buffer = nullptr;

    device->GetBackBuffer(0,
                          0,
                          D3DBACKBUFFER_TYPE_MONO,
                          &device_back_buffer);
    blit_texture(device_back_buffer,
                 references::framebuffers.get().back_buffer);
    device_back_buffer->Release();

    framebuffer_state &framebuffers = references::framebuffers.get();
    IDirect3DSurface9* front_surface = nullptr;

    ((IDirect3DTexture9*)framebuffers.front_buffer->gpu_texture.resource)
        ->GetSurfaceLevel(0, &front_surface);
    blit_texture(front_surface, framebuffers.back_buffer);
    front_surface->Release();

    device->EndScene();

    if (!references::window_inactive.read()) {
        HRESULT result = device->Present(nullptr, nullptr, nullptr, nullptr);

        static bool device_lost = false; // logging only, retail keeps no such state

        if (device_lost != (result == D3DERR_DEVICELOST)) {
            device_lost = !device_lost;

            banana::log.ngl(device_lost ? "device lost" : "device restored");
        }

        if (result == D3DERR_DEVICELOST) {
            Sleep(100);

            if (device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
                reset_device();
        }
    }

    if (references::owns_window.read())
        pump_window_messages();

    ++ngl::references::frame.get();
}

// sub_9DCF00
void ngl::d3d9::list_send_batch() {
    performance_info &performance = ngl::references::perf_info.get();

    performance.list_send_cycles = treyarch::timing::get_cpu_cycle();

    geometry_stream::begin_submission();
    
    poison_bindings();
    reset_bindings();

    IDirect3DDevice9* device = references::dev.get();

    device->BeginScene();
    render_scene(ngl::references::build_scene.read());
    device->EndScene();

    reset_bindings();
    ngl::references::end_of_frame_callback.get().invoke();

    performance.list_work_bytes_used = (u32)ngl::references::list_work_pos.read() - (u32)ngl::references::list_work.read();
    performance.list_submit_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds
            (treyarch::timing::get_cpu_cycle() - performance.list_send_cycles);

    ngl::references::end_of_render_callback.get().invoke();

    performance.cpu_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds
            (treyarch::timing::get_cpu_cycle() - performance.render_finish);

    ngl::references::end_of_vblank_callback.get().invoke();

    flip();

    performance.render_finish       = treyarch::timing::get_cpu_cycle();
    performance.frames_per_second   = 1000.0f / performance.cpu_milliseconds;
    performance.frame_milliseconds  = performance.cpu_milliseconds;
    performance.render_milliseconds = performance.cpu_milliseconds;

    performance.scratch_mesh_bytes_used = geometry_stream::bytes_used();

    if (performance.scratch_mesh_bytes_used > performance.max_scratch_mesh_bytes_used)
        performance.max_scratch_mesh_bytes_used = performance.scratch_mesh_bytes_used;

    performance.total_milliseconds += performance.cpu_milliseconds;
    performance.total_seconds      += performance.cpu_milliseconds / 1000.0f;

    debug_state &debug = ngl::references::debug.get();

    if (debug.screenshot) {
        debug::screen_shot();
        debug.screenshot = 0;
    }

    std::memcpy(&ngl::references::sync_perf_info.get(),
                &performance,
                sizeof(performance));

    performance.total_polygons = 0;
    performance.node_count     = 0;

    ngl::references::build_scene.write(nullptr);
    
    container::clear_legacy_list(references::frame_owned_objects.read());
}
