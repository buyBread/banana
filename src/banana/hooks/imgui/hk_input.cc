#include "banana/hooks/base.hh"
#include "banana/core.hh"
#include "banana/imgui/imgui.hh"
#include "util/gimmie/virt_dinput8.hh"
#include "util/gimmie/method.hh"
#include "util/vtables/IDirectInputDeviceA.hh"
#include "util/macros/lambda.hh"
#include "util/types.hh"

DEFINE_HOOK(hk_GetRawInputData, hook_signatures::imgui::GetRawInputData) {

    static void add_mouse_button_event(USHORT flags, USHORT down, USHORT up, int button) {
        if (flags & down)
            imgui::io->AddMouseButtonEvent(button, true);
        if (flags & up)
            imgui::io->AddMouseButtonEvent(button, false);
    }

public:
    CONSTRUCT_HOOK(hk_GetRawInputData,
        LAMBDA {
            return util::gimmie::method("user32.dll", "GetRawInputData");
        },
        "imgui") {}

    /*
        a saner way to do this would be via GetDeviceState with a simple
        `memset(lpvData, 0, cbData)` one-liner (+ the SetCooperativeLevel flags)
        ...unfortunately, that's not compatible with Steam Input
    */

    static UINT WINAPI detour(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader) {
        UINT result = m_original(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);

        if (!(imgui::render && imgui::io))
            return result;

        if (result == (UINT)-1)
            return result;

        if (!(pData && uiCommand == RID_INPUT))
            return result;

        RAWINPUT* input = (RAWINPUT*)pData;

        if (input->header.dwType == RIM_TYPEMOUSE) {
            const u16 button_flags =      input->data.mouse.usButtonFlags;
            const i16 wheel_delta  = (i16)input->data.mouse.usButtonData;

            if (banana::store::handle_steam_overlay) {
                add_mouse_button_event(button_flags, RI_MOUSE_LEFT_BUTTON_DOWN,   RI_MOUSE_LEFT_BUTTON_UP,   0);
                add_mouse_button_event(button_flags, RI_MOUSE_RIGHT_BUTTON_DOWN,  RI_MOUSE_RIGHT_BUTTON_UP,  1);
                add_mouse_button_event(button_flags, RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP, 2);
                add_mouse_button_event(button_flags, RI_MOUSE_BUTTON_4_DOWN,      RI_MOUSE_BUTTON_4_UP,      3);
                add_mouse_button_event(button_flags, RI_MOUSE_BUTTON_5_DOWN,      RI_MOUSE_BUTTON_5_UP,      4);
                    
                POINT point;

                if (GetCursorPos(&point) && ScreenToClient(imgui::store::handle_window, &point))
                    imgui::io->AddMousePosEvent((f32)point.x, (f32)point.y);
            }

            if ((button_flags & RI_MOUSE_WHEEL) && wheel_delta)
                imgui::io->AddMouseWheelEvent(0.f, (f32)wheel_delta / WHEEL_DELTA);

            input->data.mouse.lLastX = 0;
            input->data.mouse.lLastY = 0;
            input->data.mouse.usButtonFlags = 0;
        } else if (input->header.dwType == RIM_TYPEKEYBOARD) {
            input->data.keyboard.VKey = 0;
            input->data.keyboard.Flags |= RI_KEY_BREAK;
        }

        return result;
    }
};

DEFINE_HOOK(hk_SetCooperativeLevel, hook_signatures::imgui::SetCooperativeLevel) {
    
public:
    CONSTRUCT_HOOK(hk_SetCooperativeLevel,
        LAMBDA {
            return util::gimmie::virt_dinput8(util::vtables::IDirectInputDeviceA::SetCooperativeLevel);
        },
        "imgui") {}

    static HRESULT WINAPI detour(IDirectInputDevice8* pDevice, HWND hwnd, DWORD dwFlags) {
        // otherwise, prevents ImGui from processing inputs
        dwFlags &= ~DISCL_EXCLUSIVE;
        dwFlags |= DISCL_NONEXCLUSIVE;

        return m_original(pDevice, hwnd, dwFlags);
    }
};

CREATE_HOOK(hk_GetRawInputData);
CREATE_HOOK(hk_SetCooperativeLevel);