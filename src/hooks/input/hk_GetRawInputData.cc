#include "../base.hh"

#include "../../banana.hh"
#include "../../imgui/imgui.hh"

class hk_GetRawInputData : public c_hook<hk_GetRawInputData, banana::hk_fn_sig::GetRawInputData_t> {

    static void add_mouse_button_event(USHORT flags, USHORT down, USHORT up, int button) {
        if (flags & down) banana::imgui::io->AddMouseButtonEvent(button, true);
        if (flags & up)   banana::imgui::io->AddMouseButtonEvent(button, false);
    }

public:
    hk_GetRawInputData() : c_hook([]() -> void* { 
        return (void*)GetProcAddress(GetModuleHandleA("user32.dll"), "GetRawInputData"); 
    }) {}

    // a saner way to do this would be via GetDeviceState
    // with a simple `memset(lpvData, 0, cbData)` one-liner (+ SetCooperativeLevel flags)
    // unfortunately, that's not compatible with Steam Input

    static UINT WINAPI detour(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader) {
        UINT result = m_original(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);

        if (!(banana::imgui::render && banana::imgui::io))
            return result;

        if (result != (UINT)-1 && pData && uiCommand == RID_INPUT) {
            RAWINPUT* raw_input = static_cast<RAWINPUT*>(pData);

            if (raw_input->header.dwType == RIM_TYPEMOUSE) {
                const USHORT button_flags = raw_input->data.mouse.usButtonFlags;
                const short wheel_delta = static_cast<short>(raw_input->data.mouse.usButtonData);

                if (banana::store::handle_steam_overlay) {
                    // buttons
                    add_mouse_button_event(button_flags, RI_MOUSE_LEFT_BUTTON_DOWN,   RI_MOUSE_LEFT_BUTTON_UP,   0);
                    add_mouse_button_event(button_flags, RI_MOUSE_RIGHT_BUTTON_DOWN,  RI_MOUSE_RIGHT_BUTTON_UP,  1);
                    add_mouse_button_event(button_flags, RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP, 2);
                    add_mouse_button_event(button_flags, RI_MOUSE_BUTTON_4_DOWN,      RI_MOUSE_BUTTON_4_UP,      3);
                    add_mouse_button_event(button_flags, RI_MOUSE_BUTTON_5_DOWN,      RI_MOUSE_BUTTON_5_UP,      4);

                    // cursor
                    POINT point;

                    if (GetCursorPos(&point) && ScreenToClient(banana::store::handle_window, &point))
                        banana::imgui::io->AddMousePosEvent((float)point.x, (float)point.y);
                }

                if ((button_flags & RI_MOUSE_WHEEL) && wheel_delta)
                    banana::imgui::io->AddMouseWheelEvent(0.f, static_cast<float>(wheel_delta) / WHEEL_DELTA);

                raw_input->data.mouse.lLastX = 0;
                raw_input->data.mouse.lLastY = 0;
                raw_input->data.mouse.usButtonFlags = 0;
            } else if (raw_input->header.dwType == RIM_TYPEKEYBOARD) {
                raw_input->data.keyboard.VKey = 0;
                raw_input->data.keyboard.Flags |= RI_KEY_BREAK;
            }
        }

        return result;
    }
}; hk_GetRawInputData hk {};