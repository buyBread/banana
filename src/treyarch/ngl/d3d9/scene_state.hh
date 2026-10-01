#pragma once

#include "treyarch/ngl/scene/scene.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    scene* set_color_target(texture* target);
    scene* set_auxiliary_target(texture* target);
    scene* set_depth_target(texture* target);
    void   set_default_depth_target();

    void bind_scene_targets(scene* value);
    void apply_scene_state(scene* value);
}}} // treyarch::ngl::d3d9
