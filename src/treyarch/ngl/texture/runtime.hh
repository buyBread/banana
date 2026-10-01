#pragma once

#include <d3d9.h>

#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    enum e_runtime_texture_flags : u32 {
        runtime_texture_owned         = 0x00000020,
        runtime_texture_render_target = 0x00000040,
        runtime_texture_surface_only  = 0x00000080,
        runtime_texture_auto_depth    = 0x00000100,
        runtime_texture_named_target  = 0x00000200,
        runtime_texture_depth_texture = 0x00002000,
        runtime_texture_surface_level = 0x00004000
    };

    bool initialize_cube_resource(d3d9::texture_resource &resource,
                                  u32                     edge_length,
                                  u32                     level_count,
                                  D3DFORMAT               format,
                                  u8                      creation_flags);
    bool initialize_volume_resource(d3d9::texture_resource &resource,
                                    u32                     width,
                                    u32                     height,
                                    u32                     depth,
                                    u32                     level_count,
                                    D3DFORMAT               format,
                                    u8                      creation_flags);

    texture* create_runtime_texture(u32       flags,
                                    D3DFORMAT format,
                                    u32       width,
                                    u32       height,
                                    u32       depth,
                                    u32       level_count);

    void release_created_texture_name();

    void name_runtime_texture(texture* value, const char* name);
    void register_runtime_texture(texture* value, const fixed_string &name);

    namespace references {
        // every new runtime texture starts out named "created"
        inline util::memory_reference<fixed_string> created_texture_name       { 0x01118560 };
        inline util::memory_reference<u32>          created_texture_name_guard { 0x01118568 };
    } // references
}} // treyarch::ngl
