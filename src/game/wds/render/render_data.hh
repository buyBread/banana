#pragma once

#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct region;

    namespace ngl {
        struct mesh;
        struct mesh_section;
    } // ngl

    struct render_entry_20 {
        void* render_handle;
        void* object;
        f32   fade;
    };

    struct render_entry_30 {
        void* render_handle;
        void* object;
    };

    struct alignas(16) render_entry_40 {
        u32 object_data[3];
        u32 reserved_00c;
        f32 transform[12];
    };

    struct alignas(16) render_entry_50 {
        u32                group_index;
        ngl::mesh*         mesh;
        ngl::mesh_section* section;
        u32                reserved_00c;
        f32                position[3];
        u32                reserved_01c;
    };

    struct alignas(16) render_entry_60 {
        u32                group_index;
        u32                reserved_004;
        ngl::mesh*         mesh;
        ngl::mesh_section* section;
        f32                position[3];
        u32                reserved_01c;
    };

    struct render_entry_70 {
        u32     group_index;
        void*   owner;
        void*   subobject;
        region* reg;
    };

    class render_data {

    public:
        struct region_info {
            region* reg;
            u8      reserved_004[0x14];
        };

        struct entity_info;

        mash::vector_basic<region_info>     regions;
        mash::vector_basic<entity_info>     entities;
        mash::vector_basic<render_entry_20> entries_20;
        mash::vector_basic<render_entry_30> entries_30;
        mash::vector_basic<render_entry_40> entries_40;
        mash::vector_basic<render_entry_50> entries_50;
        mash::vector_basic<render_entry_60> entries_60;
        mash::vector_basic<render_entry_70> entries_70;
        u8                                  reserved_080[0x0C];
    };

    ASSERT_SIZEOF  (render_data::region_info,      0x18);
    ASSERT_OFFSETOF(render_data::region_info, reg, 0x00);

    ASSERT_SIZEOF(render_entry_20, 0x0C);
    ASSERT_SIZEOF(render_entry_30, 0x08);
    ASSERT_SIZEOF(render_entry_40, 0x40);
    ASSERT_SIZEOF(render_entry_50, 0x20);
    ASSERT_SIZEOF(render_entry_60, 0x20);
    ASSERT_SIZEOF(render_entry_70, 0x10);

    ASSERT_OFFSETOF(render_entry_20, fade,       0x08);
    ASSERT_OFFSETOF(render_entry_40, transform,  0x10);
    ASSERT_OFFSETOF(render_entry_50, mesh,       0x04);
    ASSERT_OFFSETOF(render_entry_50, section,    0x08);
    ASSERT_OFFSETOF(render_entry_60, mesh,       0x08);
    ASSERT_OFFSETOF(render_entry_60, section,    0x0C);
    ASSERT_OFFSETOF(render_entry_70, subobject,  0x08);

    ASSERT_SIZEOF  (render_data,             0x8C);
    ASSERT_OFFSETOF(render_data, regions,    0x00);
    ASSERT_OFFSETOF(render_data, entities,   0x10);
    ASSERT_OFFSETOF(render_data, entries_20, 0x20);
    ASSERT_OFFSETOF(render_data, entries_30, 0x30);
    ASSERT_OFFSETOF(render_data, entries_40, 0x40);
    ASSERT_OFFSETOF(render_data, entries_50, 0x50);
    ASSERT_OFFSETOF(render_data, entries_60, 0x60);
    ASSERT_OFFSETOF(render_data, entries_70, 0x70);
} // treyarch
