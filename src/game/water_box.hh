#pragma once

#include "game/math/algebra/vector3d.hh"
#include "game/wds/entity/actor.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a body of water: its box triggers say where it is and where its surface lies
    class water_box : public actor {

    public:
        // slot 158
        bool point_inside(const vector3d &point) {
            return ((bool (__thiscall*)(water_box*, const vector3d*))vtable[158])(this, &point);
        }

        // slot 160
        bool surface_point_inside(const vector3d &point) {
            return ((bool (__thiscall*)(water_box*, const vector3d*))vtable[160])(this, &point);
        }
    };
} // treyarch
