#pragma once

#include <windows.h>
#include <xinput.h>

#include "vc8/list.hh"
#include "game/input/input_device.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class rumble_manager;

    using control_function = f32 (*)(i32 control);

    namespace references {
        inline util::memory_reference<void*> input_mgr_vtable { 0x00BE9638 };

        inline util::memory_reference<DWORD (__stdcall*)(DWORD user_index, XINPUT_STATE* state)>                     XInputGetState        { 0x00B79484 };
        inline util::memory_reference<DWORD (__stdcall*)(DWORD user_index, DWORD flags, XINPUT_CAPABILITIES* caps)> XInputGetCapabilities { 0x00B79480 };

        inline util::memory_reference<bool> input_devices_scanned { 0x011136FA }; // dinput_mgr's destructor clears it
        inline util::memory_reference<bool> xenon_pad_connected   { 0x011136FB };

        inline util::memory_reference<xenon_input_device*> xenon_pad_device { 0x01113748 };
        inline util::memory_reference<input_device*>       keyjoy_device    { 0x0111374C }; // never read
    } // references

    class device_axis {

    public:
        device_id_t device;
        i32         axis;
        i32         control_axis;
    };

    class game_control {

    public:
        bool                   rational_type;
        vc8::list<device_axis> mapping;

        // sub_9763A0
        game_control() : rational_type(false) {}

        // sub_9763C0
        ~game_control() = default;
    };

    class input_mgr : public singleton<input_mgr, 0x010FC63C> {

    public:
        rumble_manager*  rumble_ptr;
        game_control     controls[128];
        u32              flags;
        bool             playing_scene_anim;
        bool             clear_signals_okay;
        bool             vibration_disabled;
        bool             keyboard_disabled;
        control_function state_monkey_callback;
        control_function delta_monkey_callback;
        device_id_t      last_used_joystick_in_menu;
        device_id_t      last_used_joystick;
        input_device*    devices[max_devices];

        input_mgr();

        static void create_inst();

        void scan_devices(); // registers a pad that came or unregisters one that went
        void poll_devices();
    };

    ASSERT_SIZEOF  (device_axis,                        0x0C);
    ASSERT_SIZEOF  (game_control,                       0x10);
    ASSERT_OFFSETOF(game_control, mapping,              0x04);

    ASSERT_SIZEOF  (input_mgr,                          0x848);
    ASSERT_OFFSETOF(input_mgr, rumble_ptr,              0x004);
    ASSERT_OFFSETOF(input_mgr, controls,                0x008);
    ASSERT_OFFSETOF(input_mgr, flags,                   0x808);
    ASSERT_OFFSETOF(input_mgr, playing_scene_anim,      0x80C);
    ASSERT_OFFSETOF(input_mgr, state_monkey_callback,   0x810);
    ASSERT_OFFSETOF(input_mgr, last_used_joystick,      0x81C);
    ASSERT_OFFSETOF(input_mgr, devices,                 0x820);
} // treyarch
