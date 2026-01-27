#pragma once

#include <windows.h>
#include <atomic>
#include <mutex>

#include "vtables.hh"
#include "log.hh"

struct IDirect3DDevice9;

namespace banana {
    extern void main();

    // when entry setup is complete
    inline std::atomic<bool> safe { false };

namespace core {
    inline std::atomic<bool> running { true };

    extern void init();
    extern void spin();
    extern void shutdown();
} // core

namespace store {
    // flags
    inline std::once_flag flag_main_thread;
    inline std::once_flag flag_hook_create_device;
    inline std::once_flag flag_hook_reset;

    // handles
    inline HMODULE handle_exe           = nullptr;
    inline HWND    handle_window        = nullptr;
    inline HMODULE handle_dxvk          = nullptr;
    inline HMODULE handle_steam_overlay = nullptr; // GameOverlayRenderer

    // misc.
    inline WNDPROC           original_WndProc = nullptr;
    inline IDirect3DDevice9* d3d9_device      = nullptr;
} // store

namespace vtables   {} // vtables.hh
namespace hk_fn_sig {} // hooks/function_signatures.hh
namespace NGL       {} // engine/ngl.hh
namespace imgui     {} // imgui/imgui.hh

} // banana

using namespace std::chrono_literals;