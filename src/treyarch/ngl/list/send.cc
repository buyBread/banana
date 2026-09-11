#include "treyarch/ngl/d3d9/submission.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/primitive_batches.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/shared/memory/memory.hh"
#include "treyarch/shared/timing/hires_clock.hh"

using namespace treyarch;

void __cdecl ngl::list_send() {
    u64 current_cycles = treyarch::timing::get_cpu_cycle();

    performance_info &performance = references::performance.get();
    
    u64 elapsed_cycles = current_cycles - performance.list_submit_cycles;

    performance.list_send_milliseconds = (f32)
        treyarch::timing::cycles_to_milliseconds(elapsed_cycles);

    debug::reset_primitive_batches();

    references::debug.get().current_node = 0;

    if (references::current_scene.read() != references::root_scene.read())
        memory::report("nglListSend called while one or more scenes were still active (need to call nglListEndScene).\n");

    d3d9::submit_list();
}
