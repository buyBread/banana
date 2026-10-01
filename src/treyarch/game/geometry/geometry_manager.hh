#pragma once

#include "treyarch/shared/math/types/matrix4x4.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

// the SM3 .ii's geometry_manager singleton, flattened into free methods and globals in retail
namespace treyarch { namespace geometry_manager {
    void set_look_at(      matrix4x4*  dest,
                     const vector3    &from,
                     const vector3    &look_at,
                     const vector3    &up);

    void set_view(const vector3 &from,
                  const vector3 &look_at,
                  const vector3 &up);

    namespace references {
        inline util::memory_reference<matrix4x4> scene_analyzer         { 0x01110730 };
        inline util::memory_reference<u8>        scene_analyzer_enabled { 0x010FB385 };

        inline util::memory_reference<matrix4x4> world_to_view   { 0x011107B0 };
        inline util::memory_reference<matrix4x4> view_to_world   { 0x011107F0 };
        inline util::memory_reference<matrix4x4> view_to_screen  { 0x01110830 };
        inline util::memory_reference<matrix4x4> world_to_screen { 0x01110870 };
    } // references
}} // treyarch::geometry_manager
