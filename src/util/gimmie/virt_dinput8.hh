#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")

namespace util { namespace gimmie {
    inline void* virt_dinput8(int idx) {
        static void** vtable = nullptr;

        if (!vtable) {
            IDirectInput8* p_directinput = nullptr;

            if (SUCCEEDED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&p_directinput, NULL))) {
                IDirectInputDevice8* p_device = nullptr;

                if (SUCCEEDED(p_directinput->CreateDevice(GUID_SysKeyboard, &p_device, NULL))) {
                    vtable = *(void***)p_device;

                    p_device->Release();
                }

                p_directinput->Release();
            }
        }
        
        return vtable ? vtable[idx] : nullptr;
    }
}} // util::gimmie

#undef DIRECTINPUT_VERSION