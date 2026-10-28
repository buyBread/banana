#pragma once

#include "retail.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/math/algebra.hh"
#include "game/math/references.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3 line_info: a segment checked against the world's collision
    class line_info {

    public:
        vector3d          start;
        vector3d          end;
        vector3d          hit_pos;
        vector3d          hit_norm;
        vector3d          local_hit_pos;              // in hit_entity's frame
        vector3d          local_hit_norm;
        arch_base_vhandle hit_entity;
        void*             hit_obb;                    // a subdivision_node_obb_base
        const void*       ent_filter;                 // the queued check's local_collision filters
        const void*       obb_filter;
        boolx             collision;
        boolx             checked_collision;
        boolx             queued_for_collision_check;
        u8                pad_05b[0x01];

        // sub_79E910
        line_info() : start(references::unk_01073f40.get()), end(references::unk_01073f40.get()), hit_entity(), collision(false), checked_collision(false), queued_for_collision_check(false) {
            clear();
        }

        // sub_4030F0
        // inlined @ sub_5C00F0
        line_info(const vector3d &s, const vector3d &e) : start(s), end(e), hit_entity(), collision(false), checked_collision(false), queued_for_collision_check(false) {
            clear();
        }

        // sub_403030
        void clear() {
            collision         = false;
            checked_collision = false;
            hit_entity        = arch_base_vhandle();
            hit_norm          = references::unk_00e60268.get();
            hit_pos           = references::unk_00fc1b20.get();
            local_hit_pos     = references::unk_00fc1b20.get();
            local_hit_norm    = references::unk_00e60268.get();

            if (queued_for_collision_check)
                retail::sub_798F10((u8*)this); // remove_to_collision_check_queue
        }
    };

    namespace references {
        // the filters als::swing_attack_move_and_face's probe checks against
        inline util::memory_reference<void*> unk_00e7b2a0 { 0x00E7B2A0 };
        inline util::memory_reference<void*> unk_00e7b2d0 { 0x00E7B2D0 };
    } // references

    ASSERT_SIZEOF  (line_info,                             0x5C);
    ASSERT_OFFSETOF(line_info, end,                        0x0C);
    ASSERT_OFFSETOF(line_info, hit_pos,                    0x18);
    ASSERT_OFFSETOF(line_info, hit_norm,                   0x24);
    ASSERT_OFFSETOF(line_info, local_hit_pos,              0x30);
    ASSERT_OFFSETOF(line_info, local_hit_norm,             0x3C);
    ASSERT_OFFSETOF(line_info, hit_entity,                 0x48);
    ASSERT_OFFSETOF(line_info, ent_filter,                 0x50);
    ASSERT_OFFSETOF(line_info, collision,                  0x58);
    ASSERT_OFFSETOF(line_info, queued_for_collision_check, 0x5A);
} // treyarch
