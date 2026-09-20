#pragma once

#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/shared/timing/hires_clock.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace amalga {
    struct resource_budget {
        hires_clock_t clock;
        f32           deadline;
    };

    struct resource_handler {
        virtual resource_handler* destroy(u8 mode) = 0;
        virtual i32 begin(i32 operation) = 0;
        virtual i32 progress(i32                  operation,
                             resource_descriptor* descriptor,
                             resource_budget*     budget) = 0;

        i32                 state;
        resource_pack_slot* pack_slot;
        e_resource_type     type;
        i32                 descriptor_cursor;

        bool advance(i32 operation, resource_budget* budget);
    };

    struct merged_apk_resource_handler : resource_handler {
        u32 entry_count;
        u32 entry_cursor;

        void begin_merged_apk(i32 operation);
        i32 progress_merged_apk(i32                  operation,
                                resource_descriptor* descriptor);
    };

    ASSERT_SIZEOF  (resource_budget,           0x10);
    ASSERT_OFFSETOF(resource_budget, deadline, 0x08);

    ASSERT_SIZEOF  (resource_handler,                    0x14);
    ASSERT_OFFSETOF(resource_handler, state,             0x04);
    ASSERT_OFFSETOF(resource_handler, pack_slot,         0x08);
    ASSERT_OFFSETOF(resource_handler, type,              0x0C);
    ASSERT_OFFSETOF(resource_handler, descriptor_cursor, 0x10);

    ASSERT_SIZEOF  (merged_apk_resource_handler,               0x1C);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_count,  0x14);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_cursor, 0x18);
}} // treyarch::amalga
