#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    void submit_list();

    namespace references {
        inline util::memory_reference<scene_callback> submission_callback_0 { 0x01115C70 };
        inline util::memory_reference<scene_callback> submission_callback_1 { 0x01115C78 };
        inline util::memory_reference<scene_callback> submission_callback_2 { 0x01115C80 };
    } // references
}}} // treyarch::ngl::d3d9
