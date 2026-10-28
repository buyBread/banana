#pragma once

#include "game/boolx.hh"
#include "game/delta_variable.hh"
#include "game/input/input_device.hh"
#include "game/math/algebra/vector2d.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class game_button;

    // a stick: its two single-axis buttons and what the update derives from them
    class game_axis : public mash::mash_virtual_base {

    public:
        device_id_t               device_id;
        f32                       lr_dead_zone;
        f32                       ud_dead_zone;
        game_button*              axis_lr;
        game_button*              axis_ud;
        delta_variable<f32>       force;       // 1 at full deflection
        delta_variable<vector2d>  stick_dir;
        delta_variable<f32>       stick_angle;
        f32                       unk_38[6];
        boolx                     suspended;
        boolx                     enabled;
        boolx                     external_axis_lr;
        boolx                     external_axis_ud;
        u8                        unk_54;
        u8                        pad_55;
        u16                       unk_56;

        bool is_enabled() const {
            return enabled.to_bool();
        }

        void enable(bool val) {
            enabled = val;
        }
    };

    ASSERT_SIZEOF  (game_axis,                   0x58);
    ASSERT_OFFSETOF(game_axis, device_id,        0x04);
    ASSERT_OFFSETOF(game_axis, force,            0x18);
    ASSERT_OFFSETOF(game_axis, stick_dir,        0x20);
    ASSERT_OFFSETOF(game_axis, stick_angle,      0x30);
    ASSERT_OFFSETOF(game_axis, suspended,        0x50);
    ASSERT_OFFSETOF(game_axis, unk_56,           0x56);
} // treyarch
