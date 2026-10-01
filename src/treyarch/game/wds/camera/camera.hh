#pragma once

#include "treyarch/game/wds/entity/entity.hh"

namespace treyarch {
    class camera : public entity {

    public:
        const vector3 &get_abs_position() const {
            return my_abs_po->get_position();
        }

        // virtual; the SM3 .ii's camera::get_fov, returns degrees
        f32 get_fov() {
            using get_fov_method = f32 (__thiscall*)(camera* self);

            return ((get_fov_method)vtable[0x28C / 4])(this);
        }
    };

    using camera_handle = camera*;

} // treyarch
