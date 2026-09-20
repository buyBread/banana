#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/shared/fixed_string.hh"

namespace treyarch { namespace ngl { namespace resources {
    void* resolve(fixed_string* name, u32 type);

    void resolve_texture(texture* &cache, const char* name);
    void resolve_effect(fx::effect* &cache, const char* name);
}}} // treyarch::ngl::resources
