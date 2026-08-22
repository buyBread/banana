#pragma once

#include "treyarch/ngl/scene/scene.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace references {
    inline util::memory_reference<scene*> current_scene { 0x01115C54 };
    inline util::memory_reference<scene*> root_scene    { 0x01115C58 };
}}} // treyarch::ngl::references
