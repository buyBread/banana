#pragma once

#include "game/wds/entity/actor.hh"

namespace treyarch {
    class camera : public actor {

    public:
        u8   reserved_0d8[0x120];
        f32  current_priority;
        u8   reserved_1fc[0x7C];
        bool unk_278; // play copies the cut scene's 0x10 flag here

        const vector3d &get_abs_position() const {
            return my_abs_po->get_position();
        }

        // slot 32; false for most cameras
        bool unk_032() {
            return ((bool (__thiscall*)(camera*))vtable[32])(this);
        }

        // virtual; the SM3 .ii's camera::get_fov, returns degrees
        f32 get_fov() {
            using get_fov_method = f32 (__thiscall*)(camera* self);

            return ((get_fov_method)vtable[0x28C / 4])(this);
        }
    };

    using camera_handle = camera*;

    ASSERT_OFFSETOF(camera, current_priority, 0x1F8);
    ASSERT_OFFSETOF(camera, unk_278,          0x278);

} // treyarch
