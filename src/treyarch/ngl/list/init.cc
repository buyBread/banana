#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/ngl/lighting/context_registry.hh"
#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/list/init.hh"
#include "treyarch/ngl/scene/dump.hh"
#include "treyarch/ngl/scene/lifecycle.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/timing/frame_timer.hh"
#include "treyarch/shared/timing/hires_clock.hh"

using namespace treyarch;

ngl::scene* __cdecl ngl::list_init() {
    timing::references::list_tick.write(timing::references::tick_state.get().tick_count);

    references::performance.get().list_submit_cycles =
        treyarch::timing::get_cpu_cycle();

    lighting::reset_context_registry();
    list::rewind();

    lighting::references::default_context.write
        (lighting::create_context());

    if (references::synchronized_debug.get().disable_missing_texture_warning)
        references::debug.get().disable_missing_texture_warning = 0;

    if (references::synchronized_debug.get().dump_scene_file) {
        references::debug.get().dump_scene_file = 0;
        
        scene_dump_start();
    }

    references::synchronized_debug.get() = references::debug.get();
    references::current_scene.write(nullptr);

    list_begin_scene(scene_parameter_defaults);
    set_scene_name("NGL Root");
    set_scene_option_group_0(false, true, false);

    return set_scene_option_group_1(false, false, false);
}
