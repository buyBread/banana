#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class finger_of_god;

    class igo_3d_radar_widget : public igo_3d_widget {

    public:
        u8                          reserved_150[0x20];
        vc8::vector<finger_of_god*> fingers_of_god;
        u8                          reserved_180[0x40];
        f32                         locked_zoom;
        u8                          reserved_1c4[0x04];
        vector3d                    locked_center;
        bool                        minimap_locked;
        bool                        buildings_invisible;
        bool                        wallmode_disabled;
        u8                          reserved_1d7[0x19];

        void lock_minimap(vector3d center, f32 zoom); // ignores a zoom of 0 or below
        void unlock_minimap();

        void set_buildings_invisible(bool invisible);
        void set_wallmode_disabled  (bool disabled);
    };

    ASSERT_SIZEOF  (igo_3d_radar_widget,                      0x1F0);
    ASSERT_OFFSETOF(igo_3d_radar_widget, fingers_of_god,      0x170);
    ASSERT_OFFSETOF(igo_3d_radar_widget, locked_zoom,         0x1C0);
    ASSERT_OFFSETOF(igo_3d_radar_widget, locked_center,       0x1C8);
    ASSERT_OFFSETOF(igo_3d_radar_widget, minimap_locked,      0x1D4);
    ASSERT_OFFSETOF(igo_3d_radar_widget, buildings_invisible, 0x1D5);
    ASSERT_OFFSETOF(igo_3d_radar_widget, wallmode_disabled,   0x1D6);
} // treyarch
