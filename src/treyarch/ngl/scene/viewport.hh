#pragma once

#include "treyarch/ngl/scene/scene.hh"

namespace treyarch { namespace ngl {
    bool __cdecl is_viewport_override_enabled();
    void __cdecl set_viewport(f32 left, f32 top, f32 right, f32 bottom);
    void __cdecl apply_active_viewport();
}} // treyarch::ngl
