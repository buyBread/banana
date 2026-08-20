#include <cstring>

#include "aspyr/win/config.hh"
#include "treyarch/ngl/d3d9/device.hh"
#include "treyarch/ngl/d3d9/display.hh"
#include "treyarch/ngl/ngl.hh"

using namespace treyarch;

void ngl::d3d9::select_default_display_mode() {
    if (references::selected_display_index.read())
        return;

    i32 selection[2] { 4, 1 };
    
    u8 index = (u8)selection[0];

    references::selected_display_mode.write
        ( (&references::display_modes.get() )[index]);
    references::selected_display_index.write(selection[0]);
}

LRESULT CALLBACK ngl::d3d9::window_procedure(HWND   window,
                                             UINT   message,
                                             WPARAM word,
                                             LPARAM long_word) {

    HWND render_window = ngl::references::render_window.read();

    switch (message) {
        case WM_GETMINMAXINFO: {
            MINMAXINFO* information = (MINMAXINFO*)long_word;

            information->ptMinTrackSize.x = 100;
            information->ptMinTrackSize.y = 100;

            return DefWindowProcA(window, message, word, long_word);
        }

        case WM_NCHITTEST:
            if (!references::windowed.read())
                return 1;

            return DefWindowProcA(window, message, word, long_word);

        case WM_KEYDOWN:
            if (word == VK_ESCAPE)
                SendMessageA(render_window, WM_CLOSE, 0, 0);

            return DefWindowProcA(window, message, word, long_word);

        case WM_SYSCOMMAND:
            if (word == SC_MAXIMIZE ||
                word == SC_SIZE ||
                word == SC_MOVE ||
                word == SC_KEYMENU ||
                word == SC_MONITORPOWER) {

                if (!references::windowed.read())
                    return 1;
            }

            return DefWindowProcA(window, message, word, long_word);

        case WM_CANCELMODE:
            if (!references::presentation.get().Windowed) {
                ShowCursor(TRUE);
                
                references::window_inactive.write(1);
            }

            return DefWindowProcA(window, message, word, long_word);

        case WM_MOVE:
            if (references::windowed.read() && render_window)
                GetWindowPlacement( render_window,
                                   &references::window_placement.get());

            return DefWindowProcA(window, message, word, long_word);

        case WM_PAINT:
            if (references::windowed.read() && references::window_inactive.read() && render_window)
                references::device.get()->Present(nullptr,
                                                  nullptr,
                                                  nullptr,
                                                  nullptr);

            return DefWindowProcA(window, message, word, long_word);

        case WM_CLOSE:
            DestroyWindow(render_window);
            ngl::references::render_window.write(nullptr);
            PostQuitMessage(0);

            return DefWindowProcA(window, message, word, long_word);

        case WM_ACTIVATEAPP:
            if (word) {
                references::window_inactive.write(0);

                if (render_window && !references::presentation.get().Windowed)
                    references::presentation.get().Windowed = TRUE;
            }

            return DefWindowProcA(window, message, word, long_word);

        default:
            return DefWindowProcA(window, message, word, long_word);
    }
}

HWND ngl::d3d9::initialize_render_window() {
    std::memset(&references::presentation.get(),
                0,
                sizeof(D3DPRESENT_PARAMETERS));

    HWND window = ngl::references::render_window.read();

    if (window)
        references::owns_window.write(0);
    else {
        references::owns_window.write(1);

        WNDCLASSEXA window_class {};

        window_class.cbSize        = sizeof(WNDCLASSEXA);
        window_class.style         = CS_OWNDC;
        window_class.lpfnWndProc   = window_procedure;
        window_class.hInstance     = GetModuleHandleA(nullptr);
        window_class.hIcon         = LoadIconA(nullptr, IDI_APPLICATION);
        window_class.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
        window_class.lpszClassName = "NGL";

        RegisterClassExA(&window_class);

        window = CreateWindowExA(0,
                                 "NGL",
                                 "NGL",
                                 0x10CF0000,
                                 0,
                                 0,
                                 0,
                                 0,
                                 nullptr,
                                 nullptr,
                                 window_class.hInstance,
                                 nullptr);

        ngl::references::render_window.write(window);
    }

    if (!references::windowed.read())
        return window;

    display_dimensions &dimensions = references::window_dimensions.get();

    if (!dimensions.width || !dimensions.height) {
        dimensions.width  = references::selected_display_mode.get().width;
        dimensions.height = references::selected_display_mode.get().height;
    }

    if (references::window_placement.get().length) {
        SetWindowPlacement(window, &references::window_placement.get());

        return window;
    }

    RECT rectangle;

    rectangle.left   = (GetSystemMetrics(SM_CXSCREEN) - dimensions.width) / 2;
    rectangle.top    = (GetSystemMetrics(SM_CYSCREEN) - dimensions.height) / 2;
    rectangle.right  = rectangle.left + dimensions.width;
    rectangle.bottom = rectangle.top + dimensions.height;

    WINDOWINFO information {};

    information.cbSize = sizeof(WINDOWINFO);

    GetWindowInfo(window, &information);
    AdjustWindowRectEx(&rectangle,
                        information.dwStyle,
                        FALSE,
                        information.dwExStyle);
    SetWindowPos(window,
                 nullptr,
                 rectangle.left,
                 rectangle.top,
                 rectangle.right - rectangle.left,
                 rectangle.bottom - rectangle.top,
                 0);

    return window;
}

void ngl::d3d9::initialize_presentation_parameters() {
    D3DPRESENT_PARAMETERS &presentation = references::presentation.get();

    display_mode &mode = references::selected_display_mode.get();

    std::memset(&presentation, 0, sizeof(D3DPRESENT_PARAMETERS));

    presentation.BackBufferWidth             = mode.width;
    presentation.BackBufferHeight            = mode.height;
    presentation.BackBufferFormat            = references::back_buffer_format.read();
    presentation.BackBufferCount             = 1;
    presentation.MultiSampleType             = D3DMULTISAMPLE_NONE;
    presentation.MultiSampleQuality          = 0;
    presentation.SwapEffect                  = D3DSWAPEFFECT_DISCARD;
    presentation.hDeviceWindow               = ngl::references::render_window.read();
    presentation.Windowed                    = references::windowed.read();
    presentation.EnableAutoDepthStencil      = TRUE;
    presentation.AutoDepthStencilFormat      = D3DFMT_D24S8;
    presentation.Flags                       = 0;
    presentation.FullScreen_RefreshRateInHz  = references::windowed.read() ?
        0 : (UINT)aspyr::win::get_config_number("VideoHz", 60.0, false);
    presentation.PresentationInterval        = aspyr::win::get_config_number("VideoVSync", 0.0, false) == 0.0 ?
            D3DPRESENT_INTERVAL_IMMEDIATE : D3DPRESENT_INTERVAL_DEFAULT;
}
