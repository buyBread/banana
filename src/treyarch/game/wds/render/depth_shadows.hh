#pragma once

#include "treyarch/shared/math/types/vector3.hh"
#include "util/types.hh"

namespace treyarch {
    struct region;

    namespace ngl {
        struct scene;
    } // ngl

    bool find_shadow_light_direction(vector3* direction,
                                     region*  current_region);

    void build_shadow_frustum_corners(      vector3*  corners,
                                      const vector3  &center,
                                      const vector3  &right,
                                      const vector3  &up,
                                      const vector3  &forward,
                                            f32       field_of_view,
                                            f32       aspect_ratio,
                                            f32       distance);

    void set_shadow_cascades(f32         distance_0,
                             f32         distance_1,
                             f32         start_0,
                             f32         start_1,
                             ngl::scene* value);

    // scene callbacks 1 and 3 of both "render_shadows" scenes, context is the cascade index
    void enter_shadow_scene(void* context);
    void leave_shadow_scene(void* context);
} // treyarch
