#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/debug/performance_display.hh"
#include "treyarch/ngl/debug/render.hh"

using namespace treyarch;

void ngl::debug::render() {
    const debug_state &state = references::synchronized_debug.get();

    if (state.show_performance_info)
        render_performance_info();

    if (state.show_performance_bar)
        render_performance_bar();
}
