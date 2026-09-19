#include <algorithm>

#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace references {
    util::memory_reference<f32*> viewport_tops    { 0x01115C40 };
    util::memory_reference<f32*> viewport_bottoms { 0x01115C44 };
    util::memory_reference<f32*> viewport_rights  { 0x01115C48 };
    util::memory_reference<f32*> viewport_lefts   { 0x01115C4C };

    util::memory_reference<u32> active_viewport_index     { 0x01115C8C };
    util::memory_reference<u8>  viewport_override_enabled { 0x01115C98 };
}}} // treyarch::ngl::references

using namespace treyarch;

bool ngl::is_viewport_override_enabled() {
    return references::viewport_override_enabled.read() != 0;
}

void ngl::set_viewport(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::current_scene.read();

    value->viewport_left   = left;
    value->viewport_top    = top;
    value->viewport_right  = right;
    value->viewport_bottom = bottom;

    references::current_scene.get()->derived_matrices_dirty = true;
}

ngl::scene* ngl::set_scissor(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::current_scene.read();

    value->scissor_left   = std::clamp(left,   -1.0f, 1.0f);
    value->scissor_top    = std::clamp(top,    -1.0f, 1.0f);
    value->scissor_right  = std::clamp(right,  -1.0f, 1.0f);
    value->scissor_bottom = std::clamp(bottom, -1.0f, 1.0f);
    value->derived_matrices_dirty = 1;

    return value;
}

ngl::scene* ngl::set_pixel_viewport(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::current_scene.read();

    validate_matrices(value);

    f32 target_width  = (f32)value->target_width;
    f32 target_height = (f32)value->target_height;

    f32 viewport_left = (f32)((f64)left / target_width  * 2.0 - 1.0);
    f32 viewport_top  = (f32)((f64)top  / target_height * 2.0 - 1.0);
    f32 viewport_right  = (f32)(((f64)right  + 1.0) / target_width  * 2.0 - 1.0);
    f32 viewport_bottom = (f32)(((f64)bottom + 1.0) / target_height * 2.0 - 1.0);

    set_viewport(viewport_left, viewport_top, viewport_right, viewport_bottom);

    return set_scissor(viewport_left, viewport_top, viewport_right, viewport_bottom);
}

void ngl::apply_active_viewport() {
    u32 active_viewport_index = references::active_viewport_index.read();

    f32 top    = references::viewport_tops   .read()[active_viewport_index];
    f32 right  = references::viewport_rights .read()[active_viewport_index];
    f32 bottom = references::viewport_bottoms.read()[active_viewport_index];
    f32 left   = references::viewport_lefts  .read()[active_viewport_index];

    set_viewport(left, top, right, bottom);
}
