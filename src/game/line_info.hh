#pragma once

#include "retail.hh"
#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/math/algebra.hh"
#include "game/math/references.hh"
#include "game/local_collision.hh"
#include "treyarch/mash/types.hh"
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

        // inlined @ sub_71F600
        explicit line_info(mash::generating_vtable_version) {}

        // sub_4030F0
        // inlined @ sub_5C00F0
        // inlined @ sub_552EC0
        // inlined @ sub_9882D0
        line_info(const vector3d &s, const vector3d &e) : start(s), end(e), hit_entity(), collision(false), checked_collision(false), queued_for_collision_check(false) {
            clear();
        }

        line_info(const line_info&) = default;

        // sub_42AD00
        line_info &operator=(const line_info &other) { // the padding stays
            start                      = other.start;
            end                        = other.end;
            hit_pos                    = other.hit_pos;
            hit_norm                   = other.hit_norm;
            local_hit_pos              = other.local_hit_pos;
            local_hit_norm             = other.local_hit_norm;
            hit_entity                 = other.hit_entity;
            hit_obb                    = other.hit_obb;
            ent_filter                 = other.ent_filter;
            obb_filter                 = other.obb_filter;
            collision                  = other.collision;
            checked_collision          = other.checked_collision;
            queued_for_collision_check = other.queued_for_collision_check;

            return *this;
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
