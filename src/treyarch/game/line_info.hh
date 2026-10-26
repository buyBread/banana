#pragma once

#include "treyarch/shared/math/algebra.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    // SM3 line_info: a segment checked against the world's collision
    class line_info {

    public:
        vector3d start;
        vector3d end;
        u8       reserved_018[0x0C];
        vector3d hit_normal;
        u8       reserved_030[0x18];
        void*    unk_048;
        u8       reserved_04c[0x0C];
        bool     collision;
        bool     unk_059;
        bool     unk_05a;
        u8       pad_05b[0x01];
    };

    namespace references {
        // the filters als::swing_attack_move_and_face's probe checks against
        inline util::memory_reference<void*> unk_00e7b2a0 { 0x00E7B2A0 };
        inline util::memory_reference<void*> unk_00e7b2d0 { 0x00E7B2D0 };
    } // references

    ASSERT_SIZEOF  (line_info,             0x5C);
    ASSERT_OFFSETOF(line_info, end,        0x0C);
    ASSERT_OFFSETOF(line_info, hit_normal, 0x24);
    ASSERT_OFFSETOF(line_info, unk_048,    0x48);
    ASSERT_OFFSETOF(line_info, collision,  0x58);
} // treyarch
