#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "treyarch/shared/math/vmath/matrix.hh"

namespace treyarch { namespace ngl {
    void calculate_matrices(scene* value);
    void validate_matrices(scene* value);

    math::mat44* get_world_to_screen(scene* value);

    void apply_matrix_plane(      math::vector4 &out,
                            const math::mat44   &matrix,
                            const math::vector4 &plane);
}} // treyarch::ngl
