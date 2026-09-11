#pragma once

#include <windows.h>
#include <d3d9.h>
#include <atomic>
#include <cstring>

#include "banana/logging.hh"

namespace banana {
    extern void thread();

    inline bool vibe_check() {
        // todo: when not incredibly lazy, don't let the bad 1.0 version of the game get past dllmain,
        //       but do it in a more "we're very fkn hardcore serious programmers here" manner
        //       w/ like MessageBox, yeah2x

        if (std::memcmp((void*)0x009CC222, "\xE8\xA9\x9A\x01\x00", 5) != 0) // ts just nglInit check :exploding_head:
            return false;

        return true;
    }

    namespace core {
        extern bool init();
        extern void spin();
        extern void shutdown();
    } // core

    namespace store {
        inline HMODULE handle_d3dx          = nullptr;
        inline HMODULE handle_dxvk          = nullptr;
        inline HMODULE handle_steam_overlay = nullptr; // GameOverlayRenderer
        inline HMODULE handle_reshade       = nullptr;

        // they're really only here for tracking them outside the bootstrapped NGL
        // should not be used in any capacity beyond reading from them
        inline IDirect3D9*       d3d9        = nullptr;
        inline IDirect3DDevice9* d3d9_device = nullptr;
    } // store
} // banana