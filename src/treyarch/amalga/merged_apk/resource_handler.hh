#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct merged_apk_resource_handler : resource_handler {
        u32 entry_count;
        u32 entry_cursor;

        void begin_merged_apk(i32 operation);
        i32 progress_merged_apk(i32                  operation,
                                resource_descriptor* descriptor);
    };

    ASSERT_SIZEOF  (merged_apk_resource_handler,               0x1C);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_count,  0x14);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_cursor, 0x18);
}} // treyarch::amalga
