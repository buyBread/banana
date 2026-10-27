#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    scene* ngli_set_render_target(texture* target);
    scene* set_auxiliary_target(texture* target);
    scene* ngli_set_z_target(texture* target);
    void   set_default_depth_target();

    void bind_scene_targets(scene* value);
    void setup_scene(scene* value);
    void render_scene(scene* value);

    namespace references {
        inline util::memory_reference<u32> scene_recursion { 0x01123AD0 };
    } // references
}}} // treyarch::ngl::d3d9
