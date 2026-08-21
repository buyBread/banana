#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    enum e_scene_parameter_source : u32 {
        scene_parameter_defaults = 0,
        scene_parameter_parent   = 1,
        scene_parameter_root     = 2
    };

    u32               get_scene_parameter_set_size();
    scene_parameters* allocate_scene_parameters();
    void              clear_scene_parameters(scene_parameters* parameters);
    void              copy_scene_parameters(      scene_parameters* destination,
                                            const scene_parameters* source);

    namespace references {
        inline ::util::memory_reference<u32> scene_parameter_count { 0x011162AC };
    } // references
}} // treyarch::ngl
