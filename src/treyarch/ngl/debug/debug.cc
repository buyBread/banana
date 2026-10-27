#include <cstring>

#include "treyarch/ngl/debug/debug.hh"

using namespace treyarch;

// sub_9E7280
void ngl::debug::init() {
    ngl::debug_state &debug_state = references::debug.get();

    std::memset(&debug_state, 0, sizeof(debug_state));

    debug_state.test_node_end = 100000;

    std::memcpy(&references::sync_debug.get(),
                &debug_state,
                sizeof(debug_state));

    ngl::performance_info &performance_info = references::perf_info.get();

    std::memset(&performance_info, 0, sizeof(performance_info));

    std::memcpy(&references::sync_perf_info.get(),
                &performance_info,
                sizeof(performance_info));

#ifdef DEBUG
    debug_state.show_performance_info = 1;
#endif
}

// sub_9E9D10
void ngl::debug::render() {
    const debug_state &state = references::sync_debug.get();

    if (state.show_performance_info)
        render_performance_info();

    if (state.show_performance_bar)
        render_performance_bar();
}
