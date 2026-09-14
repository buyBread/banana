#pragma once

#include "util/memory_reference.hh"

namespace treyarch { namespace ngl {
    struct scene;
}} // treyarch::ngl

namespace treyarch { namespace references {
    inline util::memory_reference<ngl::scene*> shadow_scene_0 { 0x01036E98 };
    inline util::memory_reference<ngl::scene*> shadow_scene_1 { 0x01036E9C };
}} // treyarch::references
