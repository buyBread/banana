#pragma once

#include "treyarch/ngl/lighting/context.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace lighting { namespace references {
    inline util::memory_reference<light_context*>   default_context        { 0x01118980 };
    inline util::memory_reference<light_context*>   current_context        { 0x01118984 };
    inline util::memory_reference<light_context*>   selected_light_context { 0x01118988 };
    inline util::memory_reference<point_light_data> point_lights           { 0x011189D0 };
}}}} // treyarch::ngl::lighting::references
