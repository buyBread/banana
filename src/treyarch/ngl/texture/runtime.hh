#pragma once

#include <d3d9.h>

#include "treyarch/ngl/texture/texture.hh"

namespace treyarch { namespace ngl {
    enum runtime_texture_flags : u32 {
        runtime_texture_owned         = 0x00000020,
        runtime_texture_render_target = 0x00000040,
        runtime_texture_surface_only  = 0x00000080,
        runtime_texture_auto_depth    = 0x00000100,
        runtime_texture_named_target  = 0x00000200,
        runtime_texture_depth_texture = 0x00002000,
        runtime_texture_surface_level = 0x00004000
    };

    texture* create_runtime_texture(u32       flags,
                                    D3DFORMAT format,
                                    u32       width,
                                    u32       height,
                                    u32       level_count);

    void name_runtime_texture(texture* value, const char* name);
    void register_runtime_texture(texture* value, const char* name);
}} // treyarch::ngl
