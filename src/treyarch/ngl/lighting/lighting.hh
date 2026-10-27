#pragma once

#include "treyarch/math/matrix.hh"
#include "game/container/legacy_list.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl {
    struct scene;
    struct texture;

namespace lighting {
    enum e_light_type : u32 {
        light_point                 = 0,
        light_directional           = 1,
        light_projected_directional = 2,
        light_user_first            = 3,
        light_generated             = 5
    };

    inline constexpr u32 light_category_general = 0x01000000;

    struct directional_light_data {
        math::vector4 direction;
        math::vector4 color;
    };

    struct point_light_data {
        math::vector4 position;
        math::vector4 color;
    };

    struct spot_light_data {
        math::vector4 position;
        math::vector4 direction;
        math::vector4 angles;
        math::vector4 color;
    };

    struct generated_light_data {
        u32           flags;
        u8            reserved_004[0x1C];
        math::vector4 direction;
        u8            reserved_030[0x10];
        math::vector4 position;
        math::mat44   projector_matrix;
        math::vector4 color;
        texture*      projector_texture;
        f32           activity;
        f32           inner_radius;
        f32           outer_radius;
        u8            reserved_0B0[0x10];
        f32           attenuation_inner;
        f32           attenuation_outer;
        u8            reserved_0C8[0x0C];
        f32           direction_w;
        u8            reserved_0D8[0x04];
        i32           projector_config_0;
        i32           projector_config_1;
        u8            reserved_0E4[0x0C];
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

    using context_registry_node = container::legacy_list_node<light_context*>;
    using context_registry      = container::legacy_list<light_context*>;

    light_context* create_light_context();
    light_context* select_build_light_context(light_context* context);
    scene*         set_light_context(light_context* context, scene* value);

    light_node* list_add_light(      u32            category,
                               const math::vector4* direction,
                               const math::vector4* color);

    void register_context(light_context* context);
    void reset_context_registry();

    namespace references {
        inline util::memory_reference<light_context*>   default_light_context { 0x01118980 };
        inline util::memory_reference<light_context*>   build_light_context   { 0x01118984 };
        inline util::memory_reference<light_context*>   send_light_context    { 0x01118988 };
        inline util::memory_reference<point_light_data> point_lights          { 0x011189D0 };
    } // references

    ASSERT_SIZEOF  (directional_light_data,            0x20);
    ASSERT_OFFSETOF(directional_light_data, direction, 0x00);
    ASSERT_OFFSETOF(directional_light_data, color,     0x10);
    ASSERT_SIZEOF  (point_light_data,                  0x20);
    ASSERT_SIZEOF  (spot_light_data,                   0x40);
    ASSERT_SIZEOF  (generated_light_data,              0xF0);

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

    ASSERT_SIZEOF(context_registry_node, 0x0C);
    ASSERT_SIZEOF(context_registry,      0x0C);
} // lighting
}} // treyarch::ngl
