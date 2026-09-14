#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct texture;

    namespace fx { namespace references {
        inline util::memory_reference<u32>      parameter_id_scene_light_source { 0x010F853C };
        inline util::memory_reference<texture*> environment_texture             { 0x010FC58C };
        inline util::memory_reference<i32>      ifl_frame                       { 0x01118800 }; // todo: not here, but w/e i'm lazy and want to go this over with for now
    } // references
}}} // treyarch::ngl::fx
