#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/boolx.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;
    class rigid_body_constraint_distance;
    class rigid_body_constraint_point;
    class user_rigid_body;

    class pendulum {

    public:
        arch_base_vhandle               entity_base_pivot;
        vector3d                        attach_point; // on the owner, where the rope ends
        vector3d                        rel_pivot_pos;
        vector3d                        abs_pivot_pos;
        f32                             constraint;   // the rope length
        f32                             adjustment_factor;
        rigid_body_constraint_distance* biped_physics_dist_constraint;
        rigid_body_constraint_point*    biped_physics_point_constraint[3];
        user_rigid_body*                pivot_rigid_body;
        i32                             current_attach_limb;
        i32                             attach_limb;
        boolx                           enabled;
        boolx                           maintain_rel_orient;
        boolx                           maintain_abs_orient;
        boolx                           unk_04f;      // the next constraint update rebuilds them

        pendulum();

        // inlined
        void enable(bool _enable) {
            enabled = _enable;
        }

        // inlined
        void set_vector3d_pivot(const vector3d &_pivot) {
            entity_base_pivot = arch_base_vhandle();
            abs_pivot_pos     = _pivot;
            rel_pivot_pos     = _pivot;
        }

        void     set_entity_base_pivot(entity* _pivot);
        vector3d get_pivot_abs_pos() const;

        // inlined
        void set_constraint(f32 _constraint) {
            constraint = _constraint;
        }

        void set_attach_limb(i32 l); // a limb past 9 is ignored

        // inlined @ sub_883AF0
        void set_maintain_rel_orient(bool on) {
            unk_04f             = maintain_rel_orient.to_bool() != on;
            maintain_rel_orient = on;
        }

        // inlined @ sub_883B70
        void set_maintain_abs_orient(bool on) {
            unk_04f             = maintain_abs_orient.to_bool() != on;
            maintain_abs_orient = on;
        }
    };

    ASSERT_OFFSETOF(pendulum, attach_point,                   0x04);
    ASSERT_OFFSETOF(pendulum, rel_pivot_pos,                  0x10);
    ASSERT_OFFSETOF(pendulum, abs_pivot_pos,                  0x1C);
    ASSERT_OFFSETOF(pendulum, constraint,                     0x28);
    ASSERT_OFFSETOF(pendulum, biped_physics_dist_constraint,  0x30);
    ASSERT_OFFSETOF(pendulum, biped_physics_point_constraint, 0x34);
    ASSERT_OFFSETOF(pendulum, pivot_rigid_body,               0x40);
    ASSERT_OFFSETOF(pendulum, attach_limb,                    0x48);
    ASSERT_OFFSETOF(pendulum, enabled,                        0x4C);
    ASSERT_OFFSETOF(pendulum, unk_04f,                        0x4F);
    ASSERT_SIZEOF(pendulum, 0x50);
} // treyarch
