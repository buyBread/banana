#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct stall_resource_handler : resource_handler_base {
        i32 stall_count;

        // sub_72E2F0
        void pre_handle_resources(e_behavior) override {
            stall_count = 1;
        }

        // sub_72E300
        i32 handle_resource(e_behavior behavior, resource_descriptor*, limited_timer*) override {
            if (behavior == load)
                return 0;

            return --stall_count < 0;
        }
    };

    ASSERT_SIZEOF  (stall_resource_handler,              0x18);
    ASSERT_OFFSETOF(stall_resource_handler, stall_count, 0x14);
}} // treyarch::amalga
