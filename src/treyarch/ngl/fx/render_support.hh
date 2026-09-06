#pragma once

#include "treyarch/ngl/fx/render_node.hh"

namespace treyarch { namespace ngl { namespace lighting {
    struct light_context;
}}}

namespace treyarch { namespace ngl { namespace fx {
    effect* select_effect(render_node* value);
    
    void record_hash_name(const fixed_string &value);

    lighting::light_context* prepare_light_context(const mesh_node_data* node_data);

    void prepare_effect_scene(effect* value);
    void prepare_effect_material(effect* value, material* material_data);
    void prepare_animated_textures(effect*         effect_data,
                                   material*       material_data,
                                   mesh_node_data* node_data);
}}} // treyarch::ngl::fx
