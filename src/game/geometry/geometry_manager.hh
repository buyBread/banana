#pragma once

#include "game/math/algebra.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

// the SM3 .ii's geometry_manager singleton, flattened into free methods and globals in retail
namespace treyarch { namespace geometry_manager {
    void set_look_at(      matrix4x4*  dest,
                     const vector3d   &from,
                     const vector3d   &look_at,
                     const vector3d   &up);

    void set_view(const vector3d &from,
                  const vector3d &look_at,
                  const vector3d &up);

    namespace references {
        inline util::memory_reference<matrix4x4> scene_analyzer         { 0x01110730 };
        inline util::memory_reference<u8>        scene_analyzer_enabled { 0x010FB385 };

        inline util::memory_reference<matrix4x4> world_to_view   { 0x011107B0 };
        inline util::memory_reference<matrix4x4> view_to_world   { 0x011107F0 };
        inline util::memory_reference<matrix4x4> view_to_screen  { 0x01110830 };
        inline util::memory_reference<matrix4x4> world_to_screen { 0x01110870 };
    } // references
}} // treyarch::geometry_manager
