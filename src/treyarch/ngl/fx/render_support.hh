#pragma once

#include "treyarch/ngl/fx/render_node.hh"

namespace treyarch { namespace ngl { namespace fx {
    void prepare_effect_scene(effect* value);
    void prepare_effect_material(effect* value, material* material_data);
    void prepare_animated_textures(effect* effect_data, shader_node* node);
}}} // treyarch::ngl::fx
