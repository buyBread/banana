#pragma once

#include "treyarch/game/light/light_manager.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<light_manager*>                        light_manager          { 0x010FC594 };
    inline util::memory_reference<dinkumware::vector<point_light_node*>*> deferred_point_lights { 0x01073EAC };
    inline util::memory_reference<dinkumware::vector<spot_light_node*>*>  deferred_spot_lights  { 0x01073EB0 };
}} // treyarch::references
