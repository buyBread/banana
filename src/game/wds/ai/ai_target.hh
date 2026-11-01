#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity_base;

    // SM3 ai_target
    class ai_target {

    public:
        // SM3's target_flags, as far as retail confirms them
        enum e_target_flags : u32 {
            target_allow_dead_target = 0x00008000
        };

        using accept_func = bool (*)(entity_base*    attacker,
                                     entity_base*    target,
                                     const vector3d &direction,
                                     const vector3d &delta,
                                     f32             dist,
                                     f32             dotp,
                                     f32*            computed_score);

        arch_base_vhandle target;
        f32               score;
        f32               threat_level;
        i32               score_results;

        ai_target();
        ai_target(arch_base_vhandle _target);

        ai_target &operator=(const ai_target &other);

        void set_target(arch_base_vhandle _target); // also resets the score, threat level and results

        bool target_valid(entity_base* attacker, u32 flags) const;

        static i32 compare_score(const void* a, const void* b); // best score first

        // SM3's parameters plus three
        f32 compute_score(      entity_base* attacker,
                          const vector3d    &pos,
                          const vector3d    &forward_vector,
                          const vector3d    &up_vector,
                                f32          min_range,
                                f32          max_range,
                                f32          min_vert_range,
                                f32          max_vert_range,
                                f32          min_dot,
                                f32          min_vert_dot,
                                u32          flags,
                                accept_func  aux_accept,
                                i32          unk_13,
                                i32          unk_14,
                                i32          unk_15);
    };

    ASSERT_SIZEOF  (ai_target,                0x10);
    ASSERT_OFFSETOF(ai_target, score,         0x04);
    ASSERT_OFFSETOF(ai_target, score_results, 0x0C);
} // treyarch
