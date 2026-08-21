#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl {
    struct scene;

namespace lighting {
    enum e_light_type : u32 {
        light_point                 = 0,
        light_directional           = 1,
        light_projected_directional = 2,
        light_user_first            = 3
    };

    struct light_node {
        light_node*  next;
        light_node*  local_next;
        u32          light_category;
        e_light_type type;
        void*        node_data;
    };

    struct light_context {
        light_node head;
        light_node projector_head;
        u8         reserved_028[0x08];
        f32        ambient[4];
        u8         platform_state_040[0x10];
    };

    light_context* create_context();
    light_context* select_context(light_context* context);
    scene*         set_scene_context(light_context* context, scene* value);

    namespace references {
        inline ::util::memory_reference<light_context*> default_context { 0x01118980 };
        inline ::util::memory_reference<light_context*> current_context { 0x01118984 };
    } // references

    ASSERT_SIZEOF  (light_node,                 0x14);
    ASSERT_OFFSETOF(light_node, next,           0x00);
    ASSERT_OFFSETOF(light_node, local_next,     0x04);
    ASSERT_OFFSETOF(light_node, light_category, 0x08);
    ASSERT_OFFSETOF(light_node, type,           0x0C);
    ASSERT_OFFSETOF(light_node, node_data,      0x10);

    ASSERT_SIZEOF  (light_context,                     0x50);
    ASSERT_OFFSETOF(light_context, head,               0x00);
    ASSERT_OFFSETOF(light_context, projector_head,     0x14);
    ASSERT_OFFSETOF(light_context, reserved_028,       0x28);
    ASSERT_OFFSETOF(light_context, ambient,            0x30);
    ASSERT_OFFSETOF(light_context, platform_state_040, 0x40);
} // lighting
}} // treyarch::ngl
