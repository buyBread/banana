#pragma once

#include "treyarch/ngl/fx/render_node.hh"

namespace treyarch { namespace ngl { namespace fx {
    bool  has_scene_parameter(const scene_parameters* parameters, u32 id);
    void* get_scene_parameter(const scene_parameters* parameters, u32 id);

    effect* select_effect(render_node* value);
    
    void record_hash_name(const fixed_string &value);

    void prepare_effect_scene(effect* value);
    void prepare_effect_material(effect* value, material* material_data);
    void prepare_animated_textures(effect*         effect_data,
                                   material*       material_data,
                                   mesh_node_data* node_data);
}}} // treyarch::ngl::fx
