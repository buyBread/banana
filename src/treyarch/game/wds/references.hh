#pragma once

#include "treyarch/game/wds/world_dynamics_system.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<world_dynamics_system*> g_world_ptr { 0x010FC54C };

    // play_scene_anim saves the global time dilation here and holds it at 1; scene_anim_dies puts it back
    inline util::memory_reference<f32> saved_global_time_dilation { 0x010FB3A4 };
}} // treyarch::references
