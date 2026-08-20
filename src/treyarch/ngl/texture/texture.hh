#pragma once

#include "treyarch/amalga/file.hh"
#include "treyarch/ngl/d3d9/texture.hh"
#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    enum texture_flags : u32 {
        texture_animated = 0x00000001,
        texture_cube     = 0x00000002,
        texture_volume   = 0x00000004
    };

    struct texture {
        amalga::file*          owner_file;
        i32                    last_frame_reference;
        fixed_string           name;
        u32                    flags;
        d3d9::texture_resource gpu_texture;
        IDirect3DSurface9*     render_target;
        texture*               depth_target;
        u32                    frame_count;
        texture**              frames;
    };

    struct texture_name {
        static string_hash get(const texture* value) {
            return value->name.hash;
        }
    };

    struct texture_directory : container::skip_list<texture, texture_name> {};

    bool can_release_texture(const texture* value);
    void process_texture(amalga::file* owner, void** mapped_sections);
    void release_texture(texture* value);

    void initialize_texture_directory();

    void __cdecl load_texture(amalga::file*       owner,
                              amalga::file_entry* entry,
                              void**              mapped_sections,
                              void*               user_data);
    void __cdecl remove_texture(amalga::file*       owner,
                                amalga::file_entry* entry,
                                void**              mapped_sections,
                                void*               user_data);

    namespace references {
        inline util::memory_reference<texture_directory> textures { 0x01118850 };
    } // references

    ASSERT_SIZEOF  (texture,                       0x44);
    ASSERT_OFFSETOF(texture, owner_file,            0x00);
    ASSERT_OFFSETOF(texture, last_frame_reference,  0x04);
    ASSERT_OFFSETOF(texture, name,                  0x08);
    ASSERT_OFFSETOF(texture, flags,                 0x10);
    ASSERT_OFFSETOF(texture, gpu_texture,           0x14);
    ASSERT_OFFSETOF(texture, render_target,         0x34);
    ASSERT_OFFSETOF(texture, depth_target,          0x38);
    ASSERT_OFFSETOF(texture, frame_count,           0x3C);
    ASSERT_OFFSETOF(texture, frames,                0x40);

    ASSERT_SIZEOF(texture_directory, 0x10);
}} // treyarch::ngl
