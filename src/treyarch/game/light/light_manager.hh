#pragma once

#include "treyarch/ngl/lighting/light_data.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct point_light_node {
        ngl::lighting::point_light_data data;
        point_light_node* next;
        u8 reserved_024[0x0C];
    };

    struct spot_light_node {
        ngl::lighting::spot_light_data data;
        spot_light_node* next;
        u8 reserved_044[0x0C];
    };

    class light_manager {

    public:
        u8 reserved_000[0x0C];
        u32 point_count;
        point_light_node* point_head;
        point_light_node* point_tail;
        u32 spot_count;
        spot_light_node* spot_head;
        spot_light_node* spot_tail;

        light_manager();
        point_light_node* add_point(const ngl::lighting::point_light_data &data);
        spot_light_node* add_spot(const ngl::lighting::spot_light_data &data);
        bool remove_point(point_light_node* node);
        bool remove_spot(spot_light_node* node);
    };

    ASSERT_SIZEOF(point_light_node, 0x30);
    ASSERT_SIZEOF(spot_light_node, 0x50);
    ASSERT_OFFSETOF(point_light_node, next, 0x20);
    ASSERT_OFFSETOF(spot_light_node, next, 0x40);
    ASSERT_SIZEOF(light_manager, 0x24);
    ASSERT_OFFSETOF(light_manager, point_count, 0x0C);
    ASSERT_OFFSETOF(light_manager, spot_count, 0x18);
} // treyarch
