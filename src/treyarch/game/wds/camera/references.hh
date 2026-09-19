#pragma once

#include "treyarch/shared/math/types/vector4.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace references {
    inline util::memory_reference<vector4> camera_right    { 0x01110770 };
    inline util::memory_reference<vector4> camera_up       { 0x01110780 };
    inline util::memory_reference<vector4> camera_forward  { 0x01110790 };
    inline util::memory_reference<vector4> camera_position { 0x011107A0 };

    inline util::memory_reference<f32> camera_aspect_ratio { 0x00F4CD50 };
    inline util::memory_reference<f32> camera_fov_radians  { 0x00F4CD54 };

    inline util::memory_reference<vector4> degenerate_axis_fallback { 0x010FC5E0 };
}} // treyarch::references
