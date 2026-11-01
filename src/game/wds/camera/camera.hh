#pragma once

#include "game/wds/entity/actor.hh"

namespace treyarch {
    class camera : public actor {

    public:
        u8  reserved_0e8[0x04];
        i32 countdown_invalid_frames;
        u8  reserved_0f0[0x20];

        // inlined @ sub_967860
        // inlined @ sub_97B440
        void invalidate(i32 n_frames_to_invalidate = 1) {
            if (n_frames_to_invalidate > countdown_invalid_frames)
                countdown_invalid_frames = n_frames_to_invalidate;
        }

        const vector3d &get_abs_position() const {
            return my_abs_po->get_position();
        }

        // slot 159
        void adjust_geometry_pipe(bool scene_analyzer = false) {
            ((void (__thiscall*)(camera*, bool))(*(void***)this)[159])(this, scene_analyzer);
        }

        // slot 160
        void unk_160() {
            ((void (__thiscall*)(camera*))(*(void***)this)[160])(this);
        }

        // slot 162
        void unk_162(bool unk) {
            ((void (__thiscall*)(camera*, bool))(*(void***)this)[162])(this, unk);
        }

        // slot 163; the SM3 .ii's camera::get_fov, returns degrees
        f32 get_fov() {
            using get_fov_method = f32 (__thiscall*)(camera* self);

            return ((get_fov_method)(*(void***)this)[163])(this);
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
            ((void (__thiscall*)(marky_camera*, const vector3d*))(*(void***)this)[171])(this, &pos);
        }

        // slot 172
        vector3d camera_get_target() {
            vector3d result;
            ((vector3d* (__thiscall*)(marky_camera*, vector3d*))(*(void***)this)[172])(this, &result);

            return result;
        }

        // slot 173
        void camera_set_roll(f32 angle) {
            ((void (__thiscall*)(marky_camera*, f32))(*(void***)this)[173])(this, angle);
        }

        // slot 177
        void camera_orbit(const vector3d &center, f32 range, f32 theta, f32 psi) {
            ((void (__thiscall*)(marky_camera*, const vector3d*, f32, f32, f32))(*(void***)this)[177])(this, &center, range, theta, psi);
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

    ASSERT_OFFSETOF(camera, countdown_invalid_frames, 0xEC);

    ASSERT_OFFSETOF(marky_camera, target,                0x1CC);
    ASSERT_OFFSETOF(marky_camera, roll,                  0x1D8);
    ASSERT_OFFSETOF(marky_camera, do_collide_with_world, 0x1DC);
    ASSERT_OFFSETOF(marky_camera, current_priority,      0x1F8);

    ASSERT_OFFSETOF(spiderman_camera, unk_278, 0x278);
} // treyarch
