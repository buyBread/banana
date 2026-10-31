#pragma once

#include "game/boolx.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;

    // SM3's find_int_flags_t, the ones read so far
    enum e_find_int_flags : u32 {
        fi_collide_camera          = 0x00000010,
        fi_collide_camera_see_thru = 0x00000100
    };

    // SM3's collision_geometry types; WoS also answers 3
    enum e_collision_geometry_type : u32 {
        collision_geometry_type_none,
        collision_geometry_type_capsule,
        collision_geometry_type_mesh
    };

    class collision_geometry {

    public:
        void** vtable;
        actor* owner;
        boolx  valid;
        boolx  dynamic;
        u8     pad_00a[0x02];
        u32    flags;        // e_find_int_flags

        // slot 19
        u32 get_type() const {
            return ((u32 (__thiscall*)(const collision_geometry*))vtable[19])(this);
        }
    };

    ASSERT_OFFSETOF(collision_geometry, owner, 0x04);
    ASSERT_OFFSETOF(collision_geometry, flags, 0x0C);
} // treyarch
