#pragma once

#include "game/math/algebra.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<vector4d> camera_right    { 0x01110770 };
    inline util::memory_reference<vector4d> camera_up       { 0x01110780 };
    inline util::memory_reference<vector4d> camera_forward  { 0x01110790 };
    inline util::memory_reference<vector4d> camera_position { 0x011107A0 };

    inline util::memory_reference<f32> camera_aspect_ratio { 0x00F4CD50 };
    inline util::memory_reference<f32> camera_fov_radians  { 0x00F4CD54 };
    inline util::memory_reference<f32> camera_near_plane   { 0x00F4CD58 };
    inline util::memory_reference<f32> camera_far_plane    { 0x00F4CD5C };

    // .rdata, also read by the chuck fov setters
    inline util::memory_reference<i32> default_fov_degrees { 0x00BE6DC4 };

    inline util::memory_reference<vector4d> degenerate_axis_fallback { 0x010FC5E0 };
}} // treyarch::references
