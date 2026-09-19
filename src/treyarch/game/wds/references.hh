#pragma once

#include "treyarch/game/wds/world_dynamics_system.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<world_dynamics_system*> g_world_ptr { 0x010FC54C };
}} // treyarch::references
