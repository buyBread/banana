#pragma once

#include "game/reference.hh"
#include "game/color.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class sm_building_lod_param {

    public:
        void* self; // the resource handler points this at the parameters themselves

        reference<ngl::texture> diffuse_edge;
        reference<ngl::texture> shininess_edge;
        reference<ngl::texture> diffuse_field;
        reference<ngl::texture> shininess_field;
        reference<ngl::texture> environment_map;
        color                   interior_color;

        reference
            <treyarch::adjustment_layers_library> adjustment_layers_library;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sm_building_lod_param,                            0x58);
    ASSERT_OFFSETOF(sm_building_lod_param, shininess_edge,            0x10);
    ASSERT_OFFSETOF(sm_building_lod_param, environment_map,           0x34);
    ASSERT_OFFSETOF(sm_building_lod_param, interior_color,            0x40);
    ASSERT_OFFSETOF(sm_building_lod_param, adjustment_layers_library, 0x50);
} // treyarch
