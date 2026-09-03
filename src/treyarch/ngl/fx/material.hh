#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/material/material.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace fx {
    struct material {
              ngl::material base;
        const char*         effect_name;
              effect*       effect_data;
              fixed_string  technique_name;
              effect*       bound_effect;
              i32           technique_index;
              i32           parameter_count;
              parameter*    parameters;
              parameter*    animated_texture_parameter_chain;
              void*         runtime_data;
    };

    void bind_material(material* value, effect* effect_data);
    void copy_material_parameters(material* value);
    void build_animated_texture_parameter_chain(effect* value);
    void build_animated_texture_parameter_chain(effect* effect_data, material* value);

    ASSERT_SIZEOF  (material,                                   0x3C);
    ASSERT_OFFSETOF(material, effect_name,                      0x14);
    ASSERT_OFFSETOF(material, effect_data,                      0x18);
    ASSERT_OFFSETOF(material, technique_name,                   0x1C);
    ASSERT_OFFSETOF(material, bound_effect,                     0x24);
    ASSERT_OFFSETOF(material, technique_index,                  0x28);
    ASSERT_OFFSETOF(material, parameter_count,                  0x2C);
    ASSERT_OFFSETOF(material, parameters,                       0x30);
    ASSERT_OFFSETOF(material, animated_texture_parameter_chain, 0x34);
    ASSERT_OFFSETOF(material, runtime_data,                     0x38);
}}} // treyarch::ngl::fx
