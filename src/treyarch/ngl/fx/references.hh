#pragma once

#include "treyarch/shared/math/types/vector4.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct texture;

    namespace fx { namespace references {
        inline util::memory_reference<u32> parameter_id_scene_light_source      { 0x010F853C };
        inline util::memory_reference<u32> parameter_id_light_source            { 0x010F7D78 };
        inline util::memory_reference<u32> parameter_id_light_table             { 0x010F7D74 };
        inline util::memory_reference<u32> parameter_id_light_table_range       { 0x01116328 };
        inline util::memory_reference<u32> parameter_id_character_color         { 0x01116304 };
        inline util::memory_reference<u32> parameter_id_parameter_subset        { 0x0111630C };
        inline util::memory_reference<u32> parameter_id_environment_color       { 0x011162F0 };
        inline util::memory_reference<u32> parameter_id_decal_projection        { 0x011162FC };
        inline util::memory_reference<u32> parameter_id_ui_parameters           { 0x01116334 };
        inline util::memory_reference<u32> parameter_id_decal_texture_matrix    { 0x01116314 };
        inline util::memory_reference<u32> parameter_id_last                    { 0x01116318 };
        inline util::memory_reference<u32> parameter_id_tint_color              { 0x01116320 };
        inline util::memory_reference<u32> parameter_id_emissive                { 0x01116324 };
        inline util::memory_reference<u32> parameter_id_morph                   { 0x0111631C };
        inline util::memory_reference<u32> parameter_id_material_texture_matrix { 0x01116300 };
        inline util::memory_reference<u32> parameter_id_material_random_seed    { 0x011162F8 };
        inline util::memory_reference<u32> parameter_id_material_scalar         { 0x011162F4 };
        inline util::memory_reference<u32> parameter_id_material_unknown_178    { 0x01116308 };
        inline util::memory_reference<u32> parameter_id_material_light_matrix   { 0x01116340 };
        inline util::memory_reference<u32> parameter_id_material_alpha          { 0x01116344 };
        inline util::memory_reference<u32> parameter_id_light_context           { 0x01116330 };
        inline util::memory_reference<u32> parameter_id_light_sphere            { 0x0111633C };
        inline util::memory_reference<u32> parameter_id_ifl_frame               { 0x01116348 };
        inline util::memory_reference<u32> parameter_id_mesh_runs               { 0x011171E0 };

        inline util::memory_reference<texture*> environment_texture { 0x010FC58C };

        // three rows per bone, written by write_bone_matrices (sub_9DE390)
        inline util::memory_reference<u32>     bone_constant_count { 0x01117168 };
        inline util::memory_reference<u8>      global_constants    { 0x01117200 };
        inline util::memory_reference<vector4> bone_constants      { 0x01117240 };
    } // references
}}} // treyarch::ngl::fx
