#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct region;

    template <typename T>
    struct render_buffer {
        u32 reserved_000;
        u32 size;
        T*  entries;
        u32 capacity;
    };

    struct render_region_info {
        region* reg;
        u8      reserved_004[0x14];
    };

    struct render_entity_info;
    struct render_entry_20;
    struct render_entry_30;
    struct render_entry_40;
    struct render_entry_50;
    struct render_entry_60;
    struct render_entry_70;

    struct render_data {
        render_buffer<render_region_info> regions;
        render_buffer<render_entity_info> entities;
        render_buffer<render_entry_20>    entries_20;
        render_buffer<render_entry_30>    entries_30;
        render_buffer<render_entry_40>    entries_40;
        render_buffer<render_entry_50>    entries_50;
        render_buffer<render_entry_60>    entries_60;
        render_buffer<render_entry_70>    entries_70;
        u8                                reserved_080[0x0C];
    };

    ASSERT_SIZEOF(render_buffer<void>, 0x10);

    ASSERT_SIZEOF  (render_region_info,      0x18);
    ASSERT_OFFSETOF(render_region_info, reg, 0x00);

    ASSERT_SIZEOF  (render_data,            0x8C);
    ASSERT_OFFSETOF(render_data, regions,    0x00);
    ASSERT_OFFSETOF(render_data, entities,   0x10);
    ASSERT_OFFSETOF(render_data, entries_20, 0x20);
    ASSERT_OFFSETOF(render_data, entries_30, 0x30);
    ASSERT_OFFSETOF(render_data, entries_40, 0x40);
    ASSERT_OFFSETOF(render_data, entries_50, 0x50);
    ASSERT_OFFSETOF(render_data, entries_60, 0x60);
    ASSERT_OFFSETOF(render_data, entries_70, 0x70);
} // treyarch
