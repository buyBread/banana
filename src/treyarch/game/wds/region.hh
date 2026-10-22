#pragma once

#include "treyarch/game/light/light_source.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class ai_pf_region_graph;
    class anchor_collection;
    class cosmetic_environments_set;
    class environment_box_set;
    class pre_light;
    class region_audio_box_set;
    class static_colgeom_root;

    namespace amalga {
        struct worldly_pack_slot;
    } // amalga

    // region load fills the resource pointers from the region's own pack
    struct region {
        u32                         unk_000; // scene entities join the region only while it's set
        u8                          reserved_004[0x14];
        anchor_collection*          anchors; // set once all of them are created
        amalga::worldly_pack_slot*  slot;
        u8                          reserved_020[0x54];
        u32*                        renderables_begin;
        u32*                        renderables_end;
        u8                          reserved_07c[0x08];
        light_source**              lights_begin;
        light_source**              lights_end;
        u8                          reserved_08c[0x38];
        static_colgeom_root*        static_colgeom;
        u8                          reserved_0c8[0x14];
        vector3d                    center;
        u8                          reserved_0e8[0x04];
        string_hash                 district_id;
        u8                          reserved_0f0[0x08];
        region_audio_box_set*       audio_boxes;
        environment_box_set*        environment_boxes;
        cosmetic_environments_set*  cosmetic_environments;
        ai_pf_region_graph*         path_graph;
        u8                          reserved_108[0x18];
        pre_light*                  prelight;
        u8                          reserved_124[0x14];
    };

    namespace references {
        // indexed by region allocation index
        inline util::memory_reference<region> region_pool { 0x01036ED8 };
    } // references

    ASSERT_SIZEOF  (region,                        0x138);
    ASSERT_OFFSETOF(region, anchors,               0x018);
    ASSERT_OFFSETOF(region, slot,                  0x01C);
    ASSERT_OFFSETOF(region, renderables_begin,     0x074);
    ASSERT_OFFSETOF(region, renderables_end,       0x078);
    ASSERT_OFFSETOF(region, lights_begin,          0x084);
    ASSERT_OFFSETOF(region, lights_end,            0x088);
    ASSERT_OFFSETOF(region, static_colgeom,        0x0C4);
    ASSERT_OFFSETOF(region, center,                0x0DC);
    ASSERT_OFFSETOF(region, district_id,           0x0EC);
    ASSERT_OFFSETOF(region, audio_boxes,           0x0F8);
    ASSERT_OFFSETOF(region, environment_boxes,     0x0FC);
    ASSERT_OFFSETOF(region, cosmetic_environments, 0x100);
    ASSERT_OFFSETOF(region, path_graph,            0x104);
    ASSERT_OFFSETOF(region, prelight,              0x120);
} // treyarch
