#pragma once

#include "treyarch/game/reference.hh"
#include "treyarch/shared/color.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/math/algebra/vector2d.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class tilemap {

    public:
        void* self; // the resource handler points this at the tilemap itself

        reference<ngl::texture>      texture;
        reference<ngl::texture>      ambient_occlusion;
        reference<ngl::texture>      normal;
        reference<ngl::texture>      interior;
        reference<treyarch::meshmap> meshmap;
        vector2d                     tile_texels;
        vector2d                     macro_texels;
        reference<ngl::texture>      diffuse;
        reference<ngl::texture>      shininess;
        reference<ngl::texture>      micronoise;
        reference<ngl::texture>      grunge;
        
        reference
            <treyarch::adjustment_layers_library> adjustment_layers_library;

        treyarch::color                      average_color;

        reference<ngl::texture>              low_lod_diffuse_map;
        reference<ngl::texture>              low_lod_shiny_map;
        i32                                  low_lod_index;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (tilemap,                            0xB0);
    ASSERT_OFFSETOF(tilemap, meshmap,                   0x34);
    ASSERT_OFFSETOF(tilemap, tile_texels,               0x3C);
    ASSERT_OFFSETOF(tilemap, diffuse,                   0x4C);
    ASSERT_OFFSETOF(tilemap, adjustment_layers_library, 0x7C);
    ASSERT_OFFSETOF(tilemap, average_color,             0x84);
    ASSERT_OFFSETOF(tilemap, low_lod_diffuse_map,       0x94);
    ASSERT_OFFSETOF(tilemap, low_lod_index,             0xAC);
} // treyarch
