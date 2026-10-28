#pragma once

#include "game/physics/physics_system.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;

    // SM3's rigid_body_prop_flags_e
    enum e_rigid_body_prop_flags : u32 {
        rigid_body_prop_flag_auto_remove_stable_prop_pieces = 0x01,
        rigid_body_prop_flag_auto_remove_empty_prop_head    = 0x02,
        rigid_body_prop_flag_non_collide_with_props         = 0x04,
        rigid_body_prop_flag_do_not_alter_collision_status  = 0x08,
        rigid_body_prop_flag_do_not_traverse_children       = 0x10
    };

    class rigid_body_prop_info {

    public:
        u8          reserved_000[0xB4];
        rigid_body* m_rb;
    };

    class rigid_body_prop_head {

    public:
        u8                    reserved_000[0x08];
        rigid_body_prop_info* m_first_rbpi;
    };

    class prop_system {

    public:
        static rigid_body_prop_head* create_prop(entity*         e,
                                                 u32             flags,
                                                 const vector3d &speed,
                                                 f32             random_variance_constant,
                                                 bool            in_collision,
                                                 void*           unk);

        static rigid_body_prop_head* find_rigid_body_prop_head(entity* e);

        static bool is_stable(rigid_body_prop_head* rbph); // every piece's body group-stable
    };

    ASSERT_OFFSETOF(rigid_body_prop_info, m_rb,         0xB4);
    ASSERT_OFFSETOF(rigid_body_prop_head, m_first_rbpi, 0x08);
} // treyarch
