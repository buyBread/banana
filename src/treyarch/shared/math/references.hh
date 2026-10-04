#pragma once

#include "treyarch/shared/math/types/vector3.hh"
#include "treyarch/shared/math/types/vector4.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace math { namespace references {
    // the SM3 .ii's XVEC, YVEC and ZVEC
    inline util::memory_reference<vector3> xvec { 0x00E6AE50 };
    inline util::memory_reference<vector3> yvec { 0x00E6AE5C };
    inline util::memory_reference<vector3> zvec { 0x00E6AE68 };

    // another (0, 1, 0); SM3 also had a vector3d_YVEC next to YVEC
    inline util::memory_reference<vector3> unk_00f4d3ac { 0x00F4D3AC };

    // (1, 0, 0), (0, 1, 0) and (0, 0, 1)
    inline util::memory_reference<vector3> unk_00f4d1e0 { 0x00F4D1E0 };
    inline util::memory_reference<vector3> unk_00f4d1ec { 0x00F4D1EC };
    inline util::memory_reference<vector3> unk_00f4d1f8 { 0x00F4D1F8 };

    // (0, 0, 0, 1); sub_5FC080 copies it in as a matrix's w row
    inline util::memory_reference<vector4> w_row { 0x00E6ADC0 };
}}} // treyarch::math::references
