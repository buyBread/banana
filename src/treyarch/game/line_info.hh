#pragma once

#include "retail.hh"
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
        vector3d hit_point;
        vector3d hit_normal;
        u8       reserved_030[0x18];
        void*    unk_048;
        u8       reserved_04c[0x0C];
        bool     collision;
        bool     unk_059;
        bool     unk_05a;
        u8       pad_05b[0x01];

        line_info() = default;

        // sub_4030F0
        // inlined @ sub_5C00F0
        line_info(const vector3d &from, const vector3d &to) : start(from), end(to), unk_048(nullptr), collision(false), unk_059(false), unk_05a(false) {
            retail::sub_403030((i32)this);
        }
    };

    namespace references {
        // the filters als::swing_attack_move_and_face's probe checks against
        inline util::memory_reference<void*> unk_00e7b2a0 { 0x00E7B2A0 };
        inline util::memory_reference<void*> unk_00e7b2d0 { 0x00E7B2D0 };
    } // references

    ASSERT_SIZEOF  (line_info,             0x5C);
    ASSERT_OFFSETOF(line_info, end,        0x0C);
    ASSERT_OFFSETOF(line_info, hit_point,  0x18);
    ASSERT_OFFSETOF(line_info, hit_normal, 0x24);
    ASSERT_OFFSETOF(line_info, unk_048,    0x48);
    ASSERT_OFFSETOF(line_info, collision,  0x58);
} // treyarch
