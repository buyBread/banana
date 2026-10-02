#pragma once

#include "treyarch/shared/stringx.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    enum device_id_t : i32 {
        joystick_1_device,
        joystick_2_device,
        joystick_3_device,
        joystick_4_device,
        joystick_5_device,
        joystick_6_device,
        joystick_7_device,
        joystick_8_device,
        keyboard_1_device,
        mouse_1_device,
        max_devices,
        current_joystick_id,
        invalid_device_id
    };

    class input_device {

        device_id_t device_id;

    public:
        virtual stringx     get_name() const = 0;
        virtual stringx     get_name(i32 axis) const = 0;
        virtual device_id_t get_id() const = 0;
        virtual i32         get_axis_count() const = 0;
        virtual i32         get_axis_id(i32 axis) const = 0;
        virtual f32         get_axis_state(i32 axis, i32 control_axis) const = 0;
        virtual f32         get_axis_old_state(i32 axis, i32 control_axis) const = 0;
        virtual f32         get_axis_delta(i32 axis, i32 control_axis) const = 0;
        virtual void        poll() = 0;
        virtual void        unk_024() = 0;
        virtual void        unk_028() = 0;
        virtual void        unk_02c() = 0;
        virtual void        unk_030() = 0;
        virtual u8          unk_034() = 0; // xenon_input_device: always false (sub_71F870)
    };

    ASSERT_SIZEOF(input_device, 0x08);
} // treyarch
