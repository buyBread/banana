#pragma once

#include <windows.h>
#include <d3d9.h>
#include <atomic>
#include <cstring>

#include "banana/logging.hh"
#include "banana/lifecycle.hh"

#define WAIT_BANANA_STATE(x) banana::store::_state.wait(x, std::memory_order_acquire);

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

        // use banana::state
        // can we OOP this too? yeah, but why?
        inline std::atomic<e_lifecycle> _state { e_lifecycle::pending };
    } // store

    namespace state {
        inline e_lifecycle current() {
            return store::_state.load(std::memory_order_acquire);
        }

        inline void update(e_lifecycle next) {
            log.dbg("banana::state: {} -> {}", current(), next);

            store::_state.store(next, std::memory_order_release);
            store::_state.notify_all();
        }
    } // state

} // banana