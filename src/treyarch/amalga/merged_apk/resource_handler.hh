#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct merged_apk_resource_handler : resource_handler_base {
        u32 entry_count;
        u32 entry_cursor;

        void pre_handle_resources(e_behavior behavior) override;

        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF  (merged_apk_resource_handler,               0x1C);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_count,  0x14);
    ASSERT_OFFSETOF(merged_apk_resource_handler, entry_cursor, 0x18);
}} // treyarch::amalga
