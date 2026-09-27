#pragma once

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shadow {
    u32 build_projection_matrices(      matrix4x4*  output,
                                  const matrix4x4  &local_to_world,
                                  const vector4    &local_sphere);
}}} // treyarch::ngl::shadow
