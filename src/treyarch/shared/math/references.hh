#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    // the SM3 .ii's XVEC, YVEC and ZVEC
    inline util::memory_reference<vector3d> xvec { 0x00E6AE50 };
    inline util::memory_reference<vector3d> yvec { 0x00E6AE5C };
    inline util::memory_reference<vector3d> zvec { 0x00E6AE68 };

    // other translation units' copies of XVEC, YVEC and ZVEC
    inline util::memory_reference<vector3d> unk_00f4d3ac { 0x00F4D3AC };
    inline util::memory_reference<vector3d> unk_00f4d1e0 { 0x00F4D1E0 };
    inline util::memory_reference<vector3d> unk_00f4d1ec { 0x00F4D1EC };
    inline util::memory_reference<vector3d> unk_00f4d1f8 { 0x00F4D1F8 };

    // (0, 0, 0); read by box triggers and other entity setup
    inline util::memory_reference<vector3d> unk_0102ff40 { 0x0102FF40 };

    // (0, 0, 0); read by animation and scene anim code
    inline util::memory_reference<vector3d> unk_00fd4370 { 0x00FD4370 };

    // one translation unit's copy of the SM3 .ii's identity_matrix
    inline util::memory_reference<matrix4x4> identity_matrix { 0x0102F770 };
}} // treyarch::references

namespace treyarch { namespace math { namespace references {
    // translation unit copies of float4.h's _f4_1000, _f4_0100 and _f4_0010
    inline util::memory_reference<float4> f4_1000 { 0x00E7CE30 };
    inline util::memory_reference<float4> f4_0100 { 0x00E7CE40 };
    inline util::memory_reference<float4> f4_0010 { 0x00E7CE50 };
}}} // treyarch::math::references
