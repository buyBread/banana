#pragma once

#include "treyarch/amalga/resource_handler.hh"

namespace treyarch { namespace amalga {
    struct merged_singular_apk_resource_handler : resource_handler_base {
        // sub_72E260
        i32 handle_resource(e_behavior, resource_descriptor*, limited_timer*) override {
            return 0;
        }
    };
}} // treyarch::amalga
