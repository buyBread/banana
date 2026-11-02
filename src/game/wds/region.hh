#pragma once

#include "game/light/light_source.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_pf_region_graph;
    class anchor_collection;
    class cosmetic_environments_set;
    class environment_box_set;
    class pre_light;
    class region_audio_box_set;
    class static_colgeom_root;
    struct collision_map_t;
    struct hierarchical_entity_proximity_map;

    namespace references {
        // no region holds it; the byte is set while it's built
        inline util::memory_reference<collision_map_t> world_collision_map       { 0x0102FFF4 };
        inline util::memory_reference<u8>              world_collision_map_built { 0x0102FEC1 };
    } // references

    namespace amalga {
        struct worldly_pack_slot;
    } // amalga

    // SM3's flags_t
    enum e_region_flags : u32 {
        region_flag_locked   = 0x00000001,
        region_flag_loaded   = 0x00000008,
        region_flag_interior = 0x00000080,
        region_flag_forced   = 0x00020000
    };

    // region load fills the resource pointers from the region's own pack
    struct region {
        collision_map_t*                    collision_proximity_map; // scene entities join the region only while it's set
        hierarchical_entity_proximity_map*  visibility_map;
        u8                                  reserved_008[0x10];
        anchor_collection*                  anchors;                 // set once all of them are created
        amalga::worldly_pack_slot*          slot;
        u8                                  reserved_020[0x54];
        u32*                                renderables_begin;
        u32*                                renderables_end;
        u8                                  reserved_07c[0x08];
        light_source**                      lights_begin;
        light_source**                      lights_end;
        u8                                  reserved_08c[0x04];
        u32                                 flags;
        u8                                  reserved_094[0x30];
        static_colgeom_root*                static_colgeom;
        u8                                  reserved_0c8[0x14];
        vector3d                            center;
        u8                                  reserved_0e8[0x04];
        string_hash                         district_id;
        u8                                  reserved_0f0[0x08];
        region_audio_box_set*               audio_boxes;
        environment_box_set*                environment_boxes;
        cosmetic_environments_set*          cosmetic_environments;
        ai_pf_region_graph*                 path_graph;
        u8                                  reserved_108[0x18];
        pre_light*                          prelight;
        u8                                  reserved_124[0x14];

        // sub_42AC80
        string_hash get_district_id() const {
            return district_id;
        }
    };

    namespace references {
        // indexed by region allocation index
        inline util::memory_reference<region> region_pool { 0x01036ED8 };
    } // references

    ASSERT_SIZEOF  (region,                         0x138);
    ASSERT_OFFSETOF(region, collision_proximity_map, 0x000);
    ASSERT_OFFSETOF(region, visibility_map,          0x004);
    ASSERT_OFFSETOF(region, anchors,                 0x018);
    ASSERT_OFFSETOF(region, slot,                    0x01C);
    ASSERT_OFFSETOF(region, renderables_begin,       0x074);
    ASSERT_OFFSETOF(region, renderables_end,         0x078);
    ASSERT_OFFSETOF(region, lights_begin,            0x084);
    ASSERT_OFFSETOF(region, lights_end,              0x088);
    ASSERT_OFFSETOF(region, flags,                   0x090);
    ASSERT_OFFSETOF(region, static_colgeom,          0x0C4);
    ASSERT_OFFSETOF(region, center,                  0x0DC);
    ASSERT_OFFSETOF(region, district_id,             0x0EC);
    ASSERT_OFFSETOF(region, audio_boxes,             0x0F8);
    ASSERT_OFFSETOF(region, environment_boxes,       0x0FC);
    ASSERT_OFFSETOF(region, cosmetic_environments,   0x100);
    ASSERT_OFFSETOF(region, path_graph,              0x104);
    ASSERT_OFFSETOF(region, prelight,                0x120);
} // treyarch
