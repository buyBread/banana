#include <cstring>

#include "treyarch/ngl/debug/debug.hh"

using namespace treyarch;

void ngl::debug::init() {
    ngl::debug_state &debug_state = references::debug.get();

    std::memset(&debug_state, 0, sizeof(debug_state));

    debug_state.test_node_end = 100000;

    std::memcpy(&references::synchronized_debug.get(),
                &debug_state,
                sizeof(debug_state));

    ngl::performance_info &performance_info = references::performance.get();

    std::memset(&performance_info, 0, sizeof(performance_info));

    std::memcpy(&references::synchronized_performance.get(),
                &performance_info,
                sizeof(performance_info));

#ifdef DEBUG
    debug_state.show_performance_info = 1;
#endif
}
