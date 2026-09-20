#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct texture;

    namespace fx { namespace references {
        inline util::memory_reference<u32> parameter_id_scene_light_source   { 0x010F853C };
        inline util::memory_reference<u32> parameter_id_light_source         { 0x010F7D78 };
        inline util::memory_reference<u32> parameter_id_light_table          { 0x010F7D74 };
        inline util::memory_reference<u32> parameter_id_light_table_range    { 0x01116328 };
        inline util::memory_reference<u32> parameter_id_character_color      { 0x01116304 };
        inline util::memory_reference<u32> parameter_id_parameter_subset     { 0x0111630C };
        inline util::memory_reference<u32> parameter_id_environment_color    { 0x011162F0 };
        inline util::memory_reference<u32> parameter_id_decal_projection     { 0x011162FC };
        inline util::memory_reference<u32> parameter_id_ui_parameters        { 0x01116334 };
        inline util::memory_reference<u32> parameter_id_decal_texture_matrix { 0x01116314 };
        inline util::memory_reference<u32> parameter_id_last                 { 0x01116318 };
        inline util::memory_reference<u32> parameter_id_tint_color           { 0x01116320 };
        inline util::memory_reference<u32> parameter_id_emissive             { 0x01116324 };
        inline util::memory_reference<u32> parameter_id_morph                { 0x0111631C };

        inline util::memory_reference<texture*> environment_texture { 0x010FC58C };

        inline util::memory_reference<i32> ifl_frame { 0x01118800 }; // todo: not here, but w/e i'm lazy and want to go this over with for now
    } // references
}}} // treyarch::ngl::fx
