#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/render_node.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;

namespace fx {
    void update_scene_parameters(effect* value);
    void update_material_parameters(effect*         value,
                                    mesh_node_data* node_data,
                                    mesh_section*   section,
                                    bool            depth_bias_enabled);
}}} // treyarch::ngl::fx
