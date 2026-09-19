#pragma once

#include "treyarch/ngl/scene/scene.hh"

namespace treyarch { namespace ngl {
    bool is_viewport_override_enabled();
    void set_viewport(f32 left, f32 top, f32 right, f32 bottom);
    scene* set_scissor(f32 left, f32 top, f32 right, f32 bottom);
    scene* set_pixel_viewport(f32 left, f32 top, f32 right, f32 bottom);
    void apply_active_viewport();
}} // treyarch::ngl
