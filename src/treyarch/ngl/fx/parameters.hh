#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/math/matrix.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;

namespace fx {
    void set_float(parameter* entry, f32 value);
    void set_int(parameter* entry, i32 value);
    void set_vector(parameter* entry, const math::vector4 &value);
    void set_matrix(parameter* entry, const math::mat44   &value);
    void set_texture(parameter* entry, texture* value);

    void update_scene_parameters(effect* value);
    void update_general_material_parameters(effect*         value,
                                            mesh_node_data* node_data,
                                            mesh_section*   section);
    void update_subset_material_parameters(effect*         value,
                                           mesh_node_data* node_data,
                                           mesh_section*   section);
    void update_subset_depth_bias_parameters(effect*         value,
                                             mesh_node_data* node_data,
                                             mesh_section*   section);
    void update_material_parameters(effect*         value,
                                    mesh_node_data* node_data,
                                    mesh_section*   section);
}}} // treyarch::ngl::fx
