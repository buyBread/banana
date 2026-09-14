#pragma once

#include "treyarch/game/render/far_away_render_list.hh"
#include "treyarch/game/render/render_data.hh"
#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct region;

    namespace ngl {
        struct scene;
    } // ngl
} // treyarch

namespace treyarch { namespace references {
    inline util::memory_reference<render_data*> render_data    { 0x010FB39C };
    inline util::memory_reference<region*>      current_region { 0x010FB398 };

    inline util::memory_reference<ngl::scene*>                 published_scene      { 0x010FC5C8 };
    inline util::memory_reference<ngl::scene*>                 z_pre_pass_scene     { 0x010FC590 };
    inline util::memory_reference<far_away_render_list_entry*> far_away_render_list { 0x010FC50C };

    inline util::memory_reference<f32> render_distance           { 0x00F4CD44 };
    inline util::memory_reference<u8>  level_render_special_case { 0x010FB384 };
    inline util::memory_reference<u8>  frame_state_1117194       { 0x01117194 };

    inline util::memory_reference<u8>  z_pre_pass_enabled         { 0x00BE4B74 };
    inline util::memory_reference<u8>  auxiliary_producer_enabled { 0x00BCD137 };
    inline util::memory_reference<u8>  auxiliary_render_enabled   { 0x00BCD138 };
    inline util::memory_reference<u8>  auxiliary_callback_enabled { 0x00BCD140 };
    inline util::memory_reference<u32> render_sequence            { 0x01073EB4 };
    inline util::memory_reference<u32> render_lock_owner          { 0x01075978 };
    inline util::memory_reference<u32> render_lock_thread         { 0x0107597C };
    inline util::memory_reference<u32> render_lock_depth          { 0x01075980 };
    inline util::memory_reference<u32> worker_generations         { 0x00FFEB30 };

    inline util::memory_reference<matrix4x4> world_to_view        { 0x011107B0 };
    inline util::memory_reference<vector4>   near_plane_reference { 0x011161D0 };
}} // treyarch::references
