#pragma once

#include <windows.h>
#include <d3d9.h>
#include <atomic>

#include "banana/logging.hh"
#include "banana/lifecycle.hh"

namespace banana {
    extern void thread();

    namespace core {
        extern bool init();
        extern void spin();
        extern void shutdown();
    } // core

    namespace store {
        inline HMODULE handle_d3dx          = nullptr;
        inline HMODULE handle_dxvk          = nullptr;
        inline HMODULE handle_steam_overlay = nullptr; // GameOverlayRenderer

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

        inline void poll() {
            store::_state.wait(current(), std::memory_order_acquire);
        }
    } // state

} // banana