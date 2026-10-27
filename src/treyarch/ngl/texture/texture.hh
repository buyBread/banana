#pragma once

#include <d3d9.h>

#include "treyarch/apk/file.hh"
#include "treyarch/ngl/gpu/d3d9/texture.hh"
#include "treyarch/base/tl_skip_list.hh"
#include "game/fixed_string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    enum e_texture_flags : u32 {
        texture_animated = 0x00000001,
        texture_cube     = 0x00000002,
        texture_volume   = 0x00000004
    };

    struct texture {
        apk::file* owner_file;
        i32                       last_frame_reference;
        fixed_string              name;
        u32                       flags;
        d3d9::texture_resource    gpu_texture;
        IDirect3DSurface9*        render_target;
        texture*                  depth_target;
        u32                       frame_count;
        texture**                 frames;
    };

    struct texture_name {
        static string_hash get(const texture* value) {
            return value->name.hash;
        }
    };

    struct texture_directory_t : tl_skip_list<texture, texture_name> {};

    bool can_release_texture(const texture* value);
    void process_texture(apk::file* owner, void** mapped_sections);
    void destroy_texture(texture* value);

    void initialize_texture_directory();

    void apk_texture_load_callback(apk::file*       owner,
                                   apk::file_entry* entry,
                                   void**           mapped_sections,
                                   void*            user_data);
    void apk_texture_delete_callback(apk::file*       owner,
                                     apk::file_entry* entry,
                                     void**           mapped_sections,
                                     void*            user_data);

    void texture_init();

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

    texture* create_texture(u32       flags,
                            D3DFORMAT format,
                            u32       width,
                            u32       height,
                            u32       depth,
                            u32       level_count);

    void release_created_texture_name();

    void name_runtime_texture(texture* value, const char* name);
    void register_runtime_texture(texture* value, const fixed_string &name);

    namespace references {
        inline util::memory_reference<texture*>            default_tex            { 0x011187FC };
        inline util::memory_reference<u32>                 animation_frame        { 0x01118800 };
        inline util::memory_reference<texture*>            white_tex              { 0x01118804 }; // "horizon_texture"
        inline util::memory_reference<texture*>            black_texture          { 0x01118808 };
        inline util::memory_reference<texture*>            invisible_texture      { 0x0111880C };
        inline util::memory_reference<fixed_string>        white_texture_name     { 0x011239B0 }; // "nglwhite", text null
        inline util::memory_reference<fixed_string>        invisible_texture_name { 0x011239B8 }; // "nglinvisible", text null
        inline util::memory_reference<fixed_string>        black_texture_name     { 0x011239C0 }; // "nglblack", text null
        inline util::memory_reference<texture_directory_t> texture_directory      { 0x01118850 };

        // every new runtime texture starts out named "created"
        inline util::memory_reference<fixed_string> created_texture_name       { 0x01118560 };
        inline util::memory_reference<u32>          created_texture_name_guard { 0x01118568 };
    } // references

    ASSERT_SIZEOF  (texture,                       0x44);
    ASSERT_OFFSETOF(texture, owner_file,           0x00);
    ASSERT_OFFSETOF(texture, last_frame_reference, 0x04);
    ASSERT_OFFSETOF(texture, name,                 0x08);
    ASSERT_OFFSETOF(texture, flags,                0x10);
    ASSERT_OFFSETOF(texture, gpu_texture,          0x14);
    ASSERT_OFFSETOF(texture, render_target,        0x34);
    ASSERT_OFFSETOF(texture, depth_target,         0x38);
    ASSERT_OFFSETOF(texture, frame_count,          0x3C);
    ASSERT_OFFSETOF(texture, frames,               0x40);

    ASSERT_SIZEOF(texture_directory_t, 0x10);
}} // treyarch::ngl
