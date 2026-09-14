#pragma once

#include "treyarch/game/world/entity.hh"

namespace treyarch {
    class camera : public entity {

    public:
        const vector3 &get_abs_position() const {
            return my_abs_po->get_position();
        }
    };

    using camera_handle = camera*;

} // treyarch
