#pragma once

#include "game/math/algebra.hh"
#include "game/math/po.hh"
#include "banana/util/memory_reference.hh"

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
    inline util::memory_reference<vector3d> unk_00e6808c { 0x00E6808C };
    inline util::memory_reference<vector3d> unk_00e68098 { 0x00E68098 };
    inline util::memory_reference<vector3d> unk_00e771dc { 0x00E771DC };
    inline util::memory_reference<vector3d> unk_00e771e8 { 0x00E771E8 };
    inline util::memory_reference<vector3d> unk_00e60268 { 0x00E60268 };
    inline util::memory_reference<vector3d> unk_00e6446c { 0x00E6446C };
    inline util::memory_reference<vector3d> unk_00e64478 { 0x00E64478 };

    // (0, 0, 0); read by box triggers and other entity setup
    inline util::memory_reference<vector3d> unk_0102ff40 { 0x0102FF40 };

    // (0, 0, 0); what line_info::clear resets the hit points to
    inline util::memory_reference<vector3d> unk_00fc1b20 { 0x00FC1B20 };

    // (0, 0, 0); read by animation and scene anim code
    inline util::memory_reference<vector3d> unk_00fd4370 { 0x00FD4370 };

    // (0, 0, 0); what a parameter without a vector and a missing vector parameter read
    inline util::memory_reference<vector3d> unk_00fc3440 { 0x00FC3440 };

    // (0, 0, 0); what an entity's vector param reads to a script when the entity has none
    inline util::memory_reference<vector3d> unk_010f7140 { 0x010F7140 };

    // (0, 0, 0), in writable data; polytube point animations start from it
    inline util::memory_reference<vector3d> unk_00ffec30 { 0x00FFEC30 };

    // (0, 0, 0, 0); a zero up vector's normalized stand-in when a scene anim binds an entity
    inline util::memory_reference<vector4d> unk_00fd4340 { 0x00FD4340 };

    // translation unit copies of the SM3 .ii's identity_matrix
    inline util::memory_reference<matrix4x4> identity_matrix { 0x0102F770 };
    inline util::memory_reference<matrix4x4> unk_01031d80    { 0x01031D80 };

    // 9.8, the world's gravity
    inline util::memory_reference<f32> gravity { 0x00E6ACE8 };

    // one translation unit's copy of the SM3 .ii's po_identity_matrix, built by po's constructor (sub_4015B0) at startup
    inline util::memory_reference<po> po_identity_matrix { 0x00FF99A0 };
}} // treyarch::references

namespace treyarch { namespace math { namespace references {
    // translation unit copies of float4.h's _f4_1000, _f4_0100 and _f4_0010
    inline util::memory_reference<float4> f4_1000 { 0x00E7CE30 };
    inline util::memory_reference<float4> f4_0100 { 0x00E7CE40 };
    inline util::memory_reference<float4> f4_0010 { 0x00E7CE50 };

    // translation unit copies of float4.h's Float4_Half, Float4_Ones and Float4_Twos
    inline util::memory_reference<float4> float4_half { 0x00E7CEB0 };
    inline util::memory_reference<float4> float4_ones { 0x00E7CEC0 };
    inline util::memory_reference<float4> float4_twos { 0x00E7CED0 };
}}} // treyarch::math::references
