#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    void render_scene(scene* value);

    namespace references {
        inline util::memory_reference<u32> scene_recursion_depth { 0x01123AD0 };
    } // references
}}} // treyarch::ngl::d3d9
