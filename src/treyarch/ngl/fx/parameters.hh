#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/fx/render_node.hh"
#include "treyarch/shared/math/vmath/matrix.hh"

namespace treyarch { namespace ngl {
    struct mesh_section;

namespace fx {
    struct general_lighting_parameters;

    void set_float(parameter* entry, f32 value);
    void set_int(parameter* entry, i32 value);
    void set_vector(parameter* entry, const math::vector4 &value);
    void set_matrix(parameter* entry, const math::mat44   &value);
    void set_texture(parameter* entry, texture* value);

    void build_general_lighting(      general_lighting_parameters* value,
                                const mesh_node_data*              node_data,
                                const mesh_section*                section);
    void update_scene_parameters(effect* value);
    void update_material_parameters(effect*         value,
                                    mesh_node_data* node_data,
                                    mesh_section*   section,
                                    bool            depth_bias_enabled);
}}} // treyarch::ngl::fx
