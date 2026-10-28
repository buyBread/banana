#pragma once

#include "game/wds/entity/actor.hh"

namespace treyarch {
    class camera : public actor {

    public:
        u8 reserved_0d8[0x38];

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

    class game_camera : public camera {

    public:
        u8 reserved_110[0xB0];
    };

    class marky_camera : public game_camera {

    public:
        u8       reserved_1c0[0x0C];
        vector3d target;
        f32      roll;
        bool     do_collide_with_world;
        u8       reserved_1dd[0x1B];
        f32      current_priority;

        // slot 171
        void camera_set_target(const vector3d &pos) {
            ((void (__thiscall*)(marky_camera*, const vector3d*))vtable[171])(this, &pos);
        }

        // slot 172
        vector3d camera_get_target() {
            vector3d result;
            ((vector3d* (__thiscall*)(marky_camera*, vector3d*))vtable[172])(this, &result);

            return result;
        }

        // slot 173
        void camera_set_roll(f32 angle) {
            ((void (__thiscall*)(marky_camera*, f32))vtable[173])(this, angle);
        }

        // slot 177
        void camera_orbit(const vector3d &center, f32 range, f32 theta, f32 psi) {
            ((void (__thiscall*)(marky_camera*, const vector3d*, f32, f32, f32))vtable[177])(this, &center, range, theta, psi);
        }

        static vector3d get_orbit_pos(const vector3d &center, f32 range, f32 theta, f32 psi);
    };

    class spiderman_camera : public game_camera {

    public:
        u8   reserved_1c0[0xB8];
        bool unk_278; // play copies the cut scene's 0x10 flag here
    };

    using camera_handle = camera*;

    ASSERT_SIZEOF(camera,      0x110);
    ASSERT_SIZEOF(game_camera, 0x1C0);

    ASSERT_OFFSETOF(marky_camera, target,                0x1CC);
    ASSERT_OFFSETOF(marky_camera, roll,                  0x1D8);
    ASSERT_OFFSETOF(marky_camera, do_collide_with_world, 0x1DC);
    ASSERT_OFFSETOF(marky_camera, current_priority,      0x1F8);

    ASSERT_OFFSETOF(spiderman_camera, unk_278, 0x278);
} // treyarch
