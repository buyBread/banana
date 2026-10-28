#pragma once

#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class rigid_body {

    public:
        u8       reserved_000[0xD0];
        vector4d m_t_vel;
        u8       reserved_0e0[0x30];
        vector4d m_force_sum;

        // inlined @ sub_86ED40
        void add_force(const vector4d &force) {
            m_force_sum.x = (f32)((f64)m_force_sum.x + force.x);
            m_force_sum.y = (f32)((f64)force.y + m_force_sum.y);
            m_force_sum.z = (f32)((f64)m_force_sum.z + force.z);
            m_force_sum.w = (f32)((f64)m_force_sum.w + 0.0);
        }

        void add_force(const vector4d &force, const vector4d &point, f32 torque_mult = 1.0f);
    };

    class nuge {

    public:
        static void apply_ballistic_target(rigid_body* const list_rigid_body[],
                                           i32                rbodies_count,
                                           const vector4d    &target,
                                           f32*               dist_sq);
    };

    ASSERT_OFFSETOF(rigid_body, m_t_vel,     0x0D0);
    ASSERT_OFFSETOF(rigid_body, m_force_sum, 0x110);
} // treyarch
