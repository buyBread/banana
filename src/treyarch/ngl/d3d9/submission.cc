#include <cstring>

#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/geometry_stream.hh"
#include "treyarch/ngl/d3d9/presentation.hh"
#include "treyarch/ngl/d3d9/scene_renderer.hh"
#include "treyarch/ngl/d3d9/submission.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/screenshot.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/shared/container/legacy_list.hh"
#include "treyarch/shared/timing/hires_clock.hh"

using namespace treyarch;

static util::memory_reference
    <container::legacy_list<void*>*> frame_owned_objects { 0x00F532A8 };

void ngl::d3d9::submit_list() {
    performance_info &performance = ngl::references::performance.get();

    performance.list_send_cycles = treyarch::timing::get_cpu_cycle();

    geometry_stream::begin_submission();
    
    poison_bindings();
    reset_bindings();

    IDirect3DDevice9* device = references::device.get();

    device->BeginScene();
    render_scene(ngl::references::current_scene.read());
    device->EndScene();

    reset_bindings();
    references::submission_callback_1.get().invoke();

    list::arena_state &arena = list::references::arena.get();

    performance.list_work_bytes_used = (u32)arena.cursor - (u32)arena.base;
    performance.list_submit_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds
            (treyarch::timing::get_cpu_cycle() - performance.list_send_cycles);

    references::submission_callback_0.get().invoke();

    performance.cpu_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds
            (treyarch::timing::get_cpu_cycle() - performance.render_finish);

    references::submission_callback_2.get().invoke();

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
        debug::capture_screenshot();
        debug.screenshot = 0;
    }

    std::memcpy(&ngl::references::synchronized_performance.get(),
                &performance,
                sizeof(performance));

    performance.total_polygons = 0;
    performance.node_count     = 0;

    ngl::references::current_scene.write(nullptr);
    
    container::clear_legacy_list(frame_owned_objects.read());
}
