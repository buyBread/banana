#define Direct3DCreate9 redefinition

#ifdef DEBUG
    #include "d3d9/IDirect3D9.hh"
#else
    #include <d3d9.h>
#endif
#include <minhook.h>

#include "util.hh"
#include "banana.hh"
#include "imgui/imgui.hh"
#include "hooks/manager.hh"

// == == == == == ==
//    ImGui SETUP
// == == == == == ==

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
    manual labour hooking without hook manager, but this code is all-setup,
    so there's no need to break it up into something more structured or w/e.
*/

typedef IDirect3D9*(WINAPI* d3d9_create_t)       (UINT SDKVersion);
typedef HRESULT    (WINAPI* d3d9_create_device_t)(IDirect3D9*, UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice9**);
typedef HRESULT    (WINAPI* d3d9_reset_t)        (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

d3d9_create_device_t original_create_device = nullptr;
d3d9_reset_t         original_reset         = nullptr;

LRESULT CALLBACK hk_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return TRUE;

    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT)
        banana::imgui::render = !banana::imgui::render;

    if (banana::imgui::render) {
        if ((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST && banana::imgui::io->WantCaptureMouse) ||
            (uMsg >= WM_KEYFIRST   && uMsg <= WM_KEYLAST   && banana::imgui::io->WantCaptureKeyboard))
            // also:
            // hooks/input/hk_GetRawInputData.cc
            // hooks/input/hk_SetCooperativeLevel.cc
            return TRUE;
    }

    return CallWindowProc(banana::store::original_WndProc, hwnd, uMsg, wParam, lParam);
}

HRESULT WINAPI hk_reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    if (banana::imgui::initialized)
        ImGui_ImplDX9_InvalidateDeviceObjects();

    HRESULT result = original_reset(pDevice, pPresentationParameters);

    if (banana::imgui::initialized && SUCCEEDED(result))
        ImGui_ImplDX9_CreateDeviceObjects();

    return result;
}

HRESULT WINAPI hk_create_device(IDirect3D9* pD3D, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) {
    HRESULT result = original_create_device(pD3D, Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    
    if (SUCCEEDED(result) && ppReturnedDeviceInterface && *ppReturnedDeviceInterface) {
        // safeguard against any odd behaviour
        if (banana::store::d3d9_device != nullptr && banana::store::d3d9_device != *ppReturnedDeviceInterface) {
            banana::safe = false;
            banana::safe.notify_all();

            SetWindowLongPtrA(
                banana::store::handle_window,
                GWLP_WNDPROC,
                (LONG_PTR)banana::store::original_WndProc
            );

            ImGui_ImplDX9_Shutdown();
            ImGui_ImplWin32_Shutdown();

            LOG.msg("game rebuilt IDirect3DDevice9");

            banana::imgui::initialized      = false;
            banana::store::original_WndProc = nullptr;
        }

        banana::store::d3d9_device   = *ppReturnedDeviceInterface;
        banana::store::handle_window = pPresentationParameters->hDeviceWindow ? pPresentationParameters->hDeviceWindow : hFocusWindow;

        LOG.msg("captured IDirect3DDevice9");

        if (!banana::imgui::initialized) {
            ImGui_ImplDX9_Init(banana::store::d3d9_device);
            ImGui_ImplWin32_Init(banana::store::handle_window);

            banana::imgui::io    = &ImGui::GetIO();
            banana::imgui::style = &ImGui::GetStyle();

            banana::imgui::io->FontDefault = banana::imgui::io->Fonts->AddFontDefaultVector();

            // banana::imgui::style->AntiAliasedLines       = false;
            // banana::imgui::style->AntiAliasedLinesUseTex = false;
            // banana::imgui::style->AntiAliasedFill        = false;

            banana::store::original_WndProc = (WNDPROC)SetWindowLongPtrA(
                banana::store::handle_window, 
                GWLP_WNDPROC, 
                (LONG_PTR)hk_WndProc
            );

            LOG.msg("ImGui initialized");

            banana::imgui::initialized = true;
        }

        // hook target doesn't change
        std::call_once(banana::store::flag_hook_reset, [&]() {
            void* target = gimmie_virt(
                banana::store::d3d9_device,
                banana::vtables::IDirect3DDevice9::Reset);
            
            MH_STATUS status = MH_CreateHook(
                target,
                (void*)&hk_reset,
                (void**)&original_reset
            );

            if (status != MH_OK)
                LOG.err("failed to create hook IDirect3DDevice9::Reset ({})", MH_STRING(status));
            
            status = MH_EnableHook(target);

            if (status != MH_OK)
                LOG.err("failed to enable hook IDirect3DDevice9::Reset ({})", MH_STRING(status));
        });

        banana::safe = true;
        banana::safe.notify_all();
    }

    return result;
}

// == == == ==
//    ENTRY
// == == == ==

#undef Direct3DCreate9

#if defined(_M_IX86)
    #pragma comment(lib, "user32.lib")
    #pragma comment(linker, "/EXPORT:Direct3DCreate9=_Direct3DCreate9@4")
#endif

extern "C" __declspec(dllexport) IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion) {
#ifndef NDEBUG
    allocate_console();
#endif
    LOG.msg("entry point");

    if (!ImGui::GetCurrentContext())
        ImGui::CreateContext();

    std::call_once(banana::store::flag_main_thread, []() {
        std::thread(banana::main).detach();
    });

    IDirect3D9* p_d3d9 = nullptr;

    banana::store::handle_exe           = GetModuleHandle(0);
    banana::store::handle_dxvk          = LoadLibraryA("dxvk.dll");
    banana::store::handle_steam_overlay = GetModuleHandle("GameOverlayRenderer.dll");

    LOG.msg("executable base: {:p}", static_cast<void*>(banana::store::handle_exe));

    if (banana::store::handle_dxvk)
        LOG.msg("using DXVK");

    if (banana::store::handle_steam_overlay)
        LOG.msg("Steam Overlay detected");

    if (!banana::store::handle_dxvk) {
        char system_directory[MAX_PATH];
        GetSystemDirectoryA(system_directory, MAX_PATH);

        p_d3d9 = ((d3d9_create_t)GetProcAddress(
            gimmie_dll("d3d9.dll"),
            "Direct3DCreate9"))(SDKVersion);
    } else
        p_d3d9 = ((d3d9_create_t)GetProcAddress(
            banana::store::handle_dxvk,
            "Direct3DCreate9"))(SDKVersion);

    MH_STATUS status = MH_Initialize();

    if (status != MH_OK && status != MH_ERROR_ALREADY_INITIALIZED) {
        LOG.err("failed to initialize minhook ({})", MH_STRING(status));

        banana::core::running = false;

        return p_d3d9; // well, at least we can still play the game
    }

    if (p_d3d9) { std::call_once(banana::store::flag_hook_create_device, [&]() {
        void* target = gimmie_virt(
            p_d3d9,
            banana::vtables::IDirect3D9::CreateDevice);
        
        status = MH_CreateHook(
            target,
            (void*)&hk_create_device,
            (void**)&original_create_device
        );

        if (status != MH_OK)
            LOG.err("failed to create hook IDirect3D9::CreateDevice ({})", MH_STRING(status));

        status = MH_EnableHook(target);

        if (status != MH_OK)
            LOG.err("failed to enable hook IDirect3D9::CreateDevice ({})", MH_STRING(status));
    }); }

#ifdef DEBUG
    return new wrapped_IDirect3D9(p_d3d9);
#else
    return p_d3d9;
#endif
}

static_assert(sizeof(void*) == 4, "ensure compile architecture is x86");