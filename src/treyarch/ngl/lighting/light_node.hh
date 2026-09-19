#pragma once

#include "treyarch/ngl/lighting/context.hh"

namespace treyarch { namespace ngl { namespace lighting {
    inline constexpr u32 light_category_general = 0x01000000;

    light_node* add_directional_light(u32            category,
                                      const vector4* direction,
                                      const vector4* color);
}}} // treyarch::ngl::lighting
