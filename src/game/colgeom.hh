#pragma once

#include "banana/util/types.hh"

namespace treyarch {
    // SM3's collision_geometry types; WoS also answers 3
    enum e_collision_geometry_type : u32 {
        collision_geometry_type_none,
        collision_geometry_type_capsule,
        collision_geometry_type_mesh
    };

    class collision_geometry {

    public:
        void** vtable;

        // slot 19
        u32 get_type() const {
            return ((u32 (__thiscall*)(const collision_geometry*))vtable[19])(this);
        }
    };
} // treyarch
