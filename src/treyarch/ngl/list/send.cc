#include <windows.h>

#include "treyarch/ngl/d3d9/submission.hh"
#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/primitive_batches.hh"
#include "treyarch/ngl/ngl.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/shared/memory/memory.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

static util::memory_reference<f32> performance_counts_per_millisecond { 0x00F51E84 };

void __cdecl ngl::list_send() {
    LARGE_INTEGER counter;

    QueryPerformanceCounter(&counter);

    performance_info &performance = references::performance.get();
    
    u64 elapsed_cycles = (u64)counter.QuadPart - performance.list_submit_cycles;

    performance.list_send_milliseconds = (f32)
        ((f64)elapsed_cycles / (f64)performance_counts_per_millisecond.read());

    debug::reset_primitive_batches();

    references::debug.get().current_node = 0;

    if (references::current_scene.read() != references::root_scene.read())
        memory::report("nglListSend called while one or more scenes were still active (need to call nglListEndScene).\n");

    d3d9::submit_list();
}
