#include <new>

#include "retail.hh"
#include "vc8/list.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/input/dinput_mgr.hh"
#include "game/input/input_mgr.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_9789E0
input_mgr::input_mgr() {
    *(void**)this = &references::input_mgr_vtable.get();

    playing_scene_anim = false;
    clear_signals_okay = false;
    vibration_disabled = false;
    keyboard_disabled  = false;
    flags              = 0;

    for (i32 index = 0; index < max_devices; ++index)
        devices[index] = nullptr;

    scan_devices();

    rumble_ptr = nullptr;

    void* allocation = mm::allocate(0x168);
    rumble_ptr = allocation ? (rumble_manager*)retail::sub_7EF6A0((u32*)allocation) : nullptr;

    playing_scene_anim         = false;
    clear_signals_okay         = false;
    last_used_joystick_in_menu = joystick_1_device;
    last_used_joystick         = joystick_1_device;
    state_monkey_callback      = nullptr;
    delta_monkey_callback      = nullptr;
}

// sub_428E90
void input_mgr::create_inst() {
    void* allocation = mm::allocate(sizeof(input_mgr));

    input_mgr::instance() = allocation ? new (allocation) input_mgr() : nullptr;
}

// sub_9CC680
void input_mgr::scan_devices() {
    vc8::list<DIDEVICEINSTANCEA> instances;
    XINPUT_STATE                 state;

    if (!references::input_devices_scanned.read()) {
        references::input_devices_scanned.write(true);

        // the first pad found becomes joystick 2, without being remembered as the xenon pad
        for (u32 pad = 0; pad < 4; ++pad) {
            if (references::XInputGetState.read()(pad, &state))
                continue;

            void* allocation = mm::allocate(0x60);
            auto* device     = allocation ? (xenon_input_device*)retail::sub_9C9DC0((u32*)allocation, pad) : nullptr;

            references::XInputGetCapabilities.read()(pad, XINPUT_FLAG_GAMEPAD, &device->capabilities);

            device->device_id = joystick_2_device;

            retail::sub_959260((u32*)this, (i32)device); // insert_device

            break;
        }

        IDirectInput8A* direct_input = dinput_mgr::inst()->direct_input;
        HRESULT         result       = direct_input->EnumDevices(0,
                                                                 (LPDIENUMDEVICESCALLBACKA)retail::sub_9CC640, // appends a copy to the list
                                                                 &instances,
                                                                 DIEDFL_ATTACHEDONLY);

        if (result)
            retail::sub_9C9690(0x8A1, result); // reports the failure

        // walked to its end and left alone
        for (auto* position = instances.begin(); position != instances.end(); position = position->next) {}

        void* allocation = mm::allocate(0x250);
        auto* keyjoy     = allocation ? (input_device*)retail::sub_9C9F60((u32*)allocation, joystick_1_device) : nullptr;

        references::keyjoy_device.write(keyjoy);

        retail::sub_959260((u32*)this, (i32)keyjoy); // insert_device
    } else if (references::frontend.get().fonts_loaded) {
        for (u32 pad = 0; pad < 4; ++pad) {
            DWORD result = references::XInputGetState.read()(pad, &state);

            if (!references::xenon_pad_connected.read()) {
                if (result)
                    continue;

                references::xenon_pad_connected.write(true);

                void* allocation = mm::allocate(0x60);
                auto* device     = allocation ? (xenon_input_device*)retail::sub_9C9DC0((u32*)allocation, pad) : nullptr;

                references::xenon_pad_device.write(device);

                references::XInputGetCapabilities.read()(pad, XINPUT_FLAG_GAMEPAD, &device->capabilities);

                device->device_id = joystick_2_device;

                retail::sub_959260((u32*)this, (i32)device); // insert_device

                references::frontend.get().refresh_igo_for_input_device();

                retail::sub_738740(); // binds the default pad buttons

                input_mgr::inst()->last_used_joystick = joystick_2_device;
            } else if (result == ERROR_DEVICE_NOT_CONNECTED && pad == 0) {
                // only the first pad going away is noticed
                retail::sub_959280((u32*)this, (i32)references::xenon_pad_device.read()); // removes and deletes it

                references::xenon_pad_device.write(nullptr);
                references::xenon_pad_connected.write(false);

                references::frontend.get().refresh_igo_for_input_device();

                retail::sub_738740(); // binds the default pad buttons

                input_mgr::inst()->last_used_joystick = joystick_1_device;
            }
        }
    }
}

// sub_9592D0
void input_mgr::poll_devices() {
    for (i32 index = 0; index != max_devices; ++index) {
        input_device* device = devices[index];

        if (device)
            device->poll();
    }
}
