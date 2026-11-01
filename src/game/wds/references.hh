#pragma once

#include "game/wds/world_dynamics_system.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<world_dynamics_system*> g_world_ptr { 0x010FC54C };

    // set once the world has advanced every als this frame; als force updates wait for it
    inline util::memory_reference<u8> unk_010fc53e { 0x010FC53E };

    // play_scene_anim saves the global time dilation here and holds it at 1; scene_anim_dies puts it back
    inline util::memory_reference<f32> saved_global_time_dilation { 0x010FB3A4 };

    // a copy of the chase camera that the hero's creation and removal and the world's destructor write; nothing reads it
    inline util::memory_reference<camera_handle> unk_01113240 { 0x01113240 };

    // the 0x28 object the common combo moves are loaded into; +0x04 holds the hero
    inline util::memory_reference<u8*> unk_010f9bd8 { 0x010F9BD8 };
}} // treyarch::references
