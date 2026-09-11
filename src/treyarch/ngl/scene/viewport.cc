#include "treyarch/ngl/scene/references.hh"
#include "treyarch/ngl/scene/viewport.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace viewport_references {
    util::memory_reference<f32*> viewport_tops    { 0x01115C40 };
    util::memory_reference<f32*> viewport_bottoms { 0x01115C44 };
    util::memory_reference<f32*> viewport_rights  { 0x01115C48 };
    util::memory_reference<f32*> viewport_lefts   { 0x01115C4C };

    util::memory_reference<u32> active_viewport_index     { 0x01115C8C };
    util::memory_reference<u8>  viewport_override_enabled { 0x01115C98 };
}}} // treyarch::ngl::viewport_references

using namespace treyarch;

bool __cdecl ngl::is_viewport_override_enabled() {
    return viewport_references::viewport_override_enabled.read() != 0;
}

void __cdecl ngl::set_viewport(f32 left, f32 top, f32 right, f32 bottom) {
    scene* value = references::current_scene.read();

    value->viewport_left   = left;
    value->viewport_top    = top;
    value->viewport_right  = right;
    value->viewport_bottom = bottom;

    references::current_scene.get()->derived_matrices_dirty = true;
}

void __cdecl ngl::apply_active_viewport() {
    u32 active_viewport_index = viewport_references::active_viewport_index.read();

    f32 top    = viewport_references::viewport_tops   .read()[active_viewport_index];
    f32 right  = viewport_references::viewport_rights .read()[active_viewport_index];
    f32 bottom = viewport_references::viewport_bottoms.read()[active_viewport_index];
    f32 left   = viewport_references::viewport_lefts  .read()[active_viewport_index];

    set_viewport(left, top, right, bottom);
}
