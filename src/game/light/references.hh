#pragma once

#include "game/light/light_manager.hh"
#include "treyarch/math/vector.hh"
#include "vc8/vector.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<light_manager*>                  light_manager         { 0x010FC594 };
    inline util::memory_reference<vc8::vector<point_light_node*>*> deferred_point_lights { 0x01073EAC };
    inline util::memory_reference<vc8::vector<spot_light_node*>*>  deferred_spot_lights  { 0x01073EB0 };

    inline util::memory_reference<math::vector4> lighting_default_value { 0x01086F30 };
    inline util::memory_reference<math::vector4> unk_01086f40           { 0x01086F40 };
    inline util::memory_reference<math::vector4> lighting_zero_value    { 0x01086F50 };
    inline util::memory_reference<math::vector4> lighting_horizon_axis  { 0x01087690 };
}} // treyarch::references
