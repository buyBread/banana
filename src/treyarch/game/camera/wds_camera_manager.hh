#pragma once

#include "treyarch/game/camera/camera.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class wds_camera_manager {

    public:
        u8            reserved_000[0x124];
        camera_handle current_camera;
        u8            reserved_128[0x18];

        camera_handle get_current_camera() const {
            return current_camera;
        }
    };

    ASSERT_SIZEOF  (wds_camera_manager,                 0x140);
    ASSERT_OFFSETOF(wds_camera_manager, current_camera, 0x124);
} // treyarch
