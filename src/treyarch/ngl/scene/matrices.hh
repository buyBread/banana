#pragma once

#include "treyarch/ngl/scene/scene.hh"

namespace treyarch { namespace ngl {
    void calculate_matrices(scene* value);
    void validate_matrices(scene* value);

    matrix4x4* get_world_to_screen(scene* value);
}} // treyarch::ngl
