#pragma once

#include "treyarch/game/wds/camera/camera.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class wds_camera_manager {

    public:
        camera_handle marky_camera;
        void*         marky_camera_vhandle;
        u8            marky_camera_enabled;
        u8            reserved_009[0x03];
        camera_handle animated_camera;
        void*         animated_camera_vhandle;
        u8            animated_camera_enabled;
        u8            reserved_015[0x03];
        u8            marky_camera_rumble_stop[0x08];
        u8            marky_camera_rumble_low[0x08];
        u8            marky_camera_rumble_medium[0x08];
        u8            marky_camera_rumble_high[0x08];

        camera_handle get_marky_camera() const {
            return marky_camera;
        }
    };

    ASSERT_SIZEOF  (wds_camera_manager,                           0x38);
    ASSERT_OFFSETOF(wds_camera_manager, marky_camera,             0x00);
    ASSERT_OFFSETOF(wds_camera_manager, marky_camera_enabled,     0x08);
    ASSERT_OFFSETOF(wds_camera_manager, animated_camera,          0x0C);
    ASSERT_OFFSETOF(wds_camera_manager, animated_camera_enabled,  0x14);
    ASSERT_OFFSETOF(wds_camera_manager, marky_camera_rumble_stop, 0x18);
} // treyarch
