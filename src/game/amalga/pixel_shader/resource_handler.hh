#pragma once

#include "game/amalga/resource_handler.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace amalga {
    // creates the payload's pixel program and files it in the shader resource manager by the resource's name
    struct pixel_shader_handler : resource_handler_base {
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF(pixel_shader_handler, 0x14);
}} // treyarch::amalga
