/*
    banana's entry point;
    because it's rather "delicate" as an entry point (also hot), it should always return a totally sane IDirect3D9
*/

// sort out including <d3d9.h> without name collision
#define Direct3DCreate9 redefinition
#ifdef DEBUG
    #pragma comment(lib, "dxguid.lib")
    #pragma comment(lib, "uuid.lib")

    // wrapped d3d9 (w/ wrapped device); for temporary debugging tasks
    // we don't use this responsibly at all
    #include "d3d9/IDirect3D9.hh"
#else
    #include <d3d9.h>
#endif
#undef Direct3DCreate9
#if defined(_M_IX86)
    #pragma comment(lib, "user32.lib")
    #pragma comment(linker, "/EXPORT:Direct3DCreate9=_Direct3DCreate9@4")
#endif

static_assert(sizeof(void*) == 4, "ensure compile architecture is 32-bit");

#include <windows.h>

#include "util/gimmie/dll.hh"
#include "util/gimmie/method.hh"
#include "util/gimmie/module.hh"
#include "banana/core.hh"
#include "banana/logging.hh"
#include "banana/hooks/manager.hh"

IDirect3D9* direct3dcreate9(HMODULE module, UINT SDKVersion) {
    auto address = util::gimmie::method(module, "Direct3DCreate9");

    // nice syntax...
    // why does it exist tho?
    return ((auto(WINAPI*)(UINT) -> IDirect3D9*)address)(SDKVersion);
}

IDirect3D9* acquire_d3d9(UINT SDKVersion) {
    IDirect3D9* d3d9 = nullptr;

    if (banana::store::handle_dxvk && !banana::store::d3d9) { // only serve DXVK to mememe
        d3d9 = direct3dcreate9(banana::store::handle_dxvk, SDKVersion);

        banana::log.dbg("serving IDirect3D9 from DXVK; SDKVersion: {}", SDKVersion);
    } else {
        d3d9 = direct3dcreate9(util::gimmie::dll("d3d9.dll"), SDKVersion);

        banana::log.dbg("serving IDirect3D9 from DirectX9; SDKVersion: {}", SDKVersion);
    }

    return d3d9;
}

extern "C" {
__declspec(dllexport) IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion) {
    using namespace banana;

    static std::once_flag init; // in case anything loads native d3d9.dll (us) over system32's

    std::call_once(init, [] {
        /*
            successful init depends on "./banana/hooks/device_lifecycle/" creating a device;
            banana failure depends on whoever calls `banana::state::update(e_lifecycle::failed)`.
        */
        std::thread(banana::thread).detach();

        store::handle_d3dx          = util::gimmie::dll("d3dx9_43.dll"); // we need this because d3dcompiler.h doesn't export D3DAssemble(??)
        store::handle_dxvk          = util::gimmie::dll_unsafe("dxvk.dll");
        store::handle_steam_overlay = util::gimmie::module_unsafe("GameOverlayRenderer.dll");

        if (store::handle_dxvk)
            banana::log.msg("DXVK detected");

        if (store::handle_steam_overlay)
            banana::log.msg("Steam Overlay detected");
    });

    IDirect3D9* result = nullptr;

    if (!store::d3d9) {
        store::d3d9 = acquire_d3d9(SDKVersion);

        result = store::d3d9; /* i don't actually know what i'm doing here... (kinda)
                                 in short though, i'm assuming that there's a chance that something might try to probe directx9 (then delete the instance);
                                 solution? fetch ours once and give it here once, then just get a new instance every other time.
                                 "will this happen?" idk. "what if the game isn't the first caller?" honestly, tough luck.
                                 i don't want to write another wrapper for this see if Release() is called before we get a device. */
        

        // also responsible for initializing ImGui
        banana::hook_manager.enable_hook("device_lifecycle", "CreateDevice");
    } else
        result = acquire_d3d9(SDKVersion);

#ifdef DEBUG
    return new wrapped_IDirect3D9(result);
#else
    return result;
#endif
}}